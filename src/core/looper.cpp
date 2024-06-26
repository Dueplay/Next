#include "core/looper.h"

#include "core/acceptor.h"
#include "core/connection.h"
#include "core/poller.h"
#include "core/thread_pool.h"
#include "log/logger.h"
namespace Next {

Looper::Looper(uint64_t timer_expiration)
    : poller_(std::make_unique<Poller>()), use_timer_(timer_expiration != 0),
      timer_expiration_(timer_expiration) {
  if (use_timer_) {
    poller_->AddConnection(timer_.GetTimerConnection());
  }
}

// 通过poller_->Poll获取epoll中就绪的事件对应的connection，然后执行他们的回调conn->GetCallback()();
void Looper::Loop() {
  while (!exit_) {
    auto ready_connections = poller_->Poll(TIMEOUT);
    Connection *timer_conn = nullptr;

    for (auto &conn : ready_connections) {
      if (conn == timer_.GetTimerConnection()) {
        timer_conn = conn;
        continue;
      }
      conn->GetCallback()();
    }

    if (timer_conn != nullptr) {
      timer_conn->GetCallback()();
    }
  }
}

void Looper::AddAcceptor(Connection *acceptor_conn) {
  std::unique_lock<std::mutex> lock(mtx_);
  poller_->AddConnection(acceptor_conn);
}

void Looper::AddConnection(std::unique_ptr<Connection> new_conn) {
  std::unique_lock<std::mutex> lock(mtx_);
  poller_->AddConnection(new_conn.get());
  int fd = new_conn->GetFd();
  connections_.insert({fd, std::move(new_conn)});
  if (use_timer_) {
    auto singletimer =
        timer_.AddSingleTimer(timer_expiration_, [this, fd = fd]() {
          LOG_INFO("client fd =" + std::to_string(fd) +
                   "has expired and will be kicked out");
          DeleteConnection(fd);
        });
    timers_mapping_.insert({fd, singletimer});
  }
}

auto Looper::RefreshConnection(int fd) noexcept -> bool {
  if (!use_timer_) {
    return false;
  }
  std::unique_lock<std::mutex> lock(mtx_);
  auto it = timers_mapping_.find(fd);
  if (use_timer_ && it != timers_mapping_.end()) {
    auto new_timer = timer_.RefreshSingleTimer(it->second, timer_expiration_);
    if (new_timer != nullptr) {
      timers_mapping_.insert({fd, new_timer});
    }
    return true;
  }
  return false;
}

auto Looper::DeleteConnection(int fd) noexcept -> bool {
  std::unique_lock<std::mutex> lock(mtx_);
  auto it = connections_.find(fd);
  if (it == connections_.end()) {
    return false;
  }
  connections_.erase(it);
  // 再删除这个连接对应的timer
  if (use_timer_) {
    auto timer_it = timers_mapping_.find(fd);
    if (timer_it != timers_mapping_.end()) {
      timer_.RemoveSingleTimer(timer_it->second);
      timers_mapping_.erase(timer_it);
    } else {
      LOG_ERROR("Looper: DeleteConnection() the fd " + std::to_string(fd) +
                " not in timers_mapping_");
    }
  }
  return true;
}

void Looper::Exit() noexcept { exit_ = true; }
} // namespace Next