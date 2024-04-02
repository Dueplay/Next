#include "core/timer.h"
#include <unistd.h>
#include <chrono>
#include <cstring>
#include "core/connection.h"
#include "core/poller.h"
#include "core/socket.h"
#include "log/logger.h"

namespace Next {

static constexpr int MILLS_IN_SECOND = 1000;
static constexpr int NANOS_IN_MILL = 1000 * 1000;

auto NowSinceEpoch() noexcept -> uint64_t {
  auto now = std::chrono::high_resolution_clock::now();
  auto duration = now.time_since_epoch();
  // 从epoch开始到now有多少ms
  return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(duration).count());
}
auto NowSinceEpoch2() noexcept -> uint64_t {
  auto now = std::chrono::high_resolution_clock::now();
  auto duration = now.time_since_epoch();
  // 从epoch开始到now有多少ns
  return static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count());
}

auto FromNow(uint64_t timestamp) noexcept -> uint64_t {
  auto now = NowSinceEpoch();
  // 从调用这个函数的时间 -> timestamp还有多少ms
  return (timestamp >= now) ? (timestamp - now) : 0;
}
// 从调用这个函数的时间 -> timestamp还有多少 秒.纳秒 s.ns
auto FromNowInTimeSpec(uint64_t timestamp) noexcept -> struct timespec {
  auto from_now_mills = FromNow(timestamp);
  struct timespec ts;
  // static_cast去掉const，将timestamp与now的差距多少ms转为 s.ns
  ts.tv_sec = static_cast<time_t>(from_now_mills / MILLS_IN_SECOND);
  ts.tv_nsec = static_cast<int64_t>((from_now_mills % MILLS_IN_SECOND) * NANOS_IN_MILL);
  return ts;
}

void ResetTimerFd(int timer_fd, struct timespec ts) {
  struct itimerspec new_time;
  struct itimerspec old_time;
  memset(&new_time, 0, sizeof(struct itimerspec));
  memset(&old_time, 0, sizeof(struct itimerspec));
  new_time.it_value = ts;

  int ret = timerfd_settime(timer_fd, 0, &new_time, &old_time);
  if (ret < 0) {
    LOG_ERROR("ResetTimerFd(): timerfd_settime fails");
  }
}

// 从构造的时间 + expire_from_now作为超时的时间点expire_time_
Timer::SingleTimer::SingleTimer(uint64_t expire_from_now, std::function<void()> callback) noexcept
    : expire_time_(NowSinceEpoch() + expire_from_now), callback_(std::move(callback)) {}

auto Timer::SingleTimer::WhenExpire() const noexcept -> uint64_t { return expire_time_; }

auto Timer::SingleTimer::Expired() const noexcept -> bool { return NowSinceEpoch() >= expire_time_; }

void Timer::SingleTimer::Run() noexcept {
  if (callback_) {
    callback_();
  }
}

auto Timer::SingleTimer::GetCallback() const noexcept -> std::function<void()> { return callback_; }

Timer::Timer() : timer_fd_(timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC)) {
  if (timer_fd_ < 0) {
    LOG_FATAL("Timer() : timerfd_create fails");
    exit(EXIT_FAILURE);
  }
  timer_conn_ = std::make_unique<Connection>(std::make_unique<Socket>(timer_fd_));
  timer_conn_->SetEvents(POLL_READ | POLL_ET);
  //  bind eg:print(int a, int b), auto boundFunc = std::bind(print, std::placeholders::_1, 42);
  //  boundFunc 是一个新的可调用对象，它接受一个参数作为 print 函数的第一个参数，而第二个参数则是在绑定时已经指定为 42
  timer_conn_->SetCallback(std::bind(&Timer::HandleRead, this));
}

auto Timer::GetTimerConnection() -> Connection * { return timer_conn_.get(); }

auto Timer::GetTimerFd() -> int { return timer_fd_; }

auto Timer::AddSingleTimer(uint64_t expire_from_now, const std::function<void()> &callback) noexcept -> SingleTimer * {
  std::unique_lock<std::mutex> lock(mtx_);  // 对queue实现互斥访问
  auto new_timer = std::make_unique<SingleTimer>(expire_from_now, callback);
  auto raw_timer = new_timer.get();
  timer_queue_.emplace(raw_timer, std::move(new_timer));
  uint64_t new_next_expire = NextExpireTime();
  if (new_next_expire != next_expire_) {
    next_expire_ = new_next_expire;
    ResetTimerFd(timer_fd_,
                 FromNowInTimeSpec(next_expire_));  // 添加新的timer后，下次超时时间变了，修改为新的
  }
  return raw_timer;
}

auto Timer::RemoveSingleTimer(Timer::SingleTimer *single_timer) noexcept -> bool {
  std::unique_lock<std::mutex> lock(mtx_);
  auto it = timer_queue_.find(single_timer);
  if (it != timer_queue_.end()) {
    timer_queue_.erase(it);
    // 删除一个timer后，看下次超时时间是否改变
    uint64_t new_next_expire = NextExpireTime();
    if (new_next_expire != next_expire_) {
      next_expire_ = new_next_expire;
      ResetTimerFd(timer_fd_,
                   FromNowInTimeSpec(next_expire_));  // 下次超时时间变了，修改为新的
    }
    return true;
  }
  return false;
}

// 更新SingleTimer队列中某个SingleTimer的expire_from_now
auto Timer::RefreshSingleTimer(Timer::SingleTimer *single_timer, uint64_t expire_from_now) noexcept
    -> Timer::SingleTimer * {
  std::unique_lock<std::mutex> lock(mtx_);
  auto it = timer_queue_.find(single_timer);
  if (it == timer_queue_.end()) {
    return nullptr;
  }
  auto new_timer = std::make_unique<SingleTimer>(expire_from_now, single_timer->GetCallback());
  auto raw_timer = new_timer.get();
  timer_queue_.erase(it);
  timer_queue_.emplace(raw_timer, std::move(new_timer));
  uint64_t new_next_expire = NextExpireTime();
  if (new_next_expire != next_expire_) {
    next_expire_ = new_next_expire;
    ResetTimerFd(timer_fd_,
                 FromNowInTimeSpec(next_expire_));  // 添加新的timer后，下次超时时间变了，修改为新的
  }
  return raw_timer;
}

auto Timer::NextExpireTime() const noexcept -> uint64_t {
  if (timer_queue_.empty()) return 0;
  return timer_queue_.begin()->first->WhenExpire();
}

auto Timer::TimerCount() const noexcept -> size_t { return timer_queue_.size(); }

auto Timer::PruneExpiredTimer() noexcept -> std::vector<std::unique_ptr<SingleTimer>> {
  std::unique_lock<std::mutex> lock(mtx_);
  std::vector<std::unique_ptr<SingleTimer>> expired;
  auto it = timer_queue_.begin();
  for (; it != timer_queue_.end(); it++) {
    if (!it->first->Expired()) {
      break;
    }
  }
  // begin till it has expired and we are to remove them
  for (auto expired_it = timer_queue_.begin(); expired_it != it; expired_it++) {
    expired.push_back(std::move(expired_it->second));
  }
  timer_queue_.erase(timer_queue_.begin(), it);

  uint64_t new_next_expire = NextExpireTime();
  if (new_next_expire != next_expire_) {
    next_expire_ = new_next_expire;
    ResetTimerFd(timer_fd_,
                 FromNowInTimeSpec(next_expire_));  // 移出超时的定时器，下次超时时间变了，修改为新的
  }
  return expired;
}

void Timer::HandleRead() {
  uint64_t expired_count;
  /*
  当定时器超时，timer_fd_上read读事件发生即可读，返回超时次数（从上次调用timerfd_settime()启动开始或上次read成功读取开始），
  它是一个8字节的unit64_t类型整数，如果定时器没有发生超时事件，则read将阻塞；若timerfd为阻塞模式，否则返回EAGAIN
  错误（O_NONBLOCK模式）； 如果read时提供的缓冲区小于8字节将以EINVAL错误返回。

  */
  ssize_t n = read(timer_fd_, &expired_count, sizeof expired_count);
  if (n != sizeof expired_count) {
    LOG_ERROR("Timer: HandleRead() read from timer_fd doesn't get a byte of 8");
  }
  // 执行到期的定时器的回调
  auto expired_timer = PruneExpiredTimer();
  for (const auto &single_expired : expired_timer) {
    single_expired->Run();
  }
}

// map中超时时间从小到大排序
auto Timer::SingleTimerCompartor::operator()(const SingleTimer *lhs, const SingleTimer *rhs) const noexcept -> bool {
  return lhs->WhenExpire() < rhs->WhenExpire();
}

}  // namespace Next