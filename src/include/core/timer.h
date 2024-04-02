#ifndef NEXT_TIMER_H
#define NEXT_TIMER_H

#include <sys/timerfd.h>
#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

namespace Next {
class Socket;
class Connection;
auto NowSinceEpoch() noexcept -> uint64_t;
auto NowSinceEpoch2() noexcept -> uint64_t;

auto FromNow(uint64_t timestamp) noexcept -> uint64_t;

auto FromNowInTimeSpec(uint64_t timestamp) noexcept -> struct timespec;

void ResetTimerFd(int timer_fd, struct timespec ts);

class Timer {
public:
    class SingleTimer{
        public:
            SingleTimer(uint64_t expire_from_now, std::function<void()> callback) noexcept;

            auto WhenExpire() const noexcept -> uint64_t;

            auto Expired() const noexcept -> bool;

            void Run() noexcept;

            auto GetCallback() const noexcept -> std::function<void()>;
        private:
            uint64_t expire_time_;
            std::function<void()> callback_{nullptr};
    };

    Timer();
    auto GetTimerConnection() -> Connection *;

    auto GetTimerFd() -> int;

    auto AddSingleTimer(uint64_t expire_from_now, const std::function<void()> &callback) noexcept -> SingleTimer *;

    auto RemoveSingleTimer(SingleTimer *single_timer) noexcept -> bool;

    auto RefreshSingleTimer(SingleTimer *single_timer, uint64_t expire_from_now) noexcept -> Timer::SingleTimer *;

    auto NextExpireTime() const noexcept -> uint64_t;

    auto TimerCount() const noexcept -> size_t;

    auto PruneExpiredTimer() noexcept -> std::vector<std::unique_ptr<SingleTimer>>;
private:
    struct SingleTimerCompartor {
        auto operator()(const SingleTimer *lhs, const SingleTimer *rhs) const noexcept -> bool;
    };

    void HandleRead();
    
    int timer_fd_;
    uint64_t next_expire_{0};
    mutable std::mutex mtx_;
    std::unique_ptr<Connection> timer_conn_;
    std::map<SingleTimer *, std::unique_ptr<SingleTimer>, SingleTimerCompartor> timer_queue_;
};
}
#endif