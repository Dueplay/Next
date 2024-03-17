#ifndef Next_LOOPER_H
#define Next_LOOPER_H

#include <atomic>
#include <cstdint>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <mutex>

#include "core/timer.h"
#include "core/utils.h"

namespace Next {

static constexpr int TIMEOUT = 3000;  // ms

static constexpr uint64_t INACTIVE_TIMEOUT = 3000;  // 单位ms 一个Connection必须在这个时间内完成

class Poller;

class ThreadPool;

class Connection;

class Acceptor;

class Looper {
 public:
  explicit Looper(uint64_t timer_expiration = 0);

  ~Looper() = default;

  NON_COPYABLE(Looper);

  void Loop();

  void AddAcceptor(Connection *acceptor_conn);

  void AddConnection(std::unique_ptr<Connection> new_conn);

  auto RefreshConnection(int fd) noexcept -> bool;

  auto DeleteConnection(int fd) noexcept -> bool;

  void Exit() noexcept;

 private:
  std::unique_ptr<Poller> poller_;
  std::mutex mtx_;
  std::map<int, std::unique_ptr<Connection>> connections_;
  std::map<int, Timer::SingleTimer *> timers_mapping_;
  Timer timer_{};
  bool exit_{false};
  bool use_timer_{false};
  uint64_t timer_expiration_{0};
};

}  // namespace Next
#endif