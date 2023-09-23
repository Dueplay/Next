#include <algorithm>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
/* all header files included */
#include "core/acceptor.h"
#include "core/buffer.h"
#include "core/cache.h"
#include "core/connection.h"
#include "core/looper.h"
#include "core/net_addr.h"
#include "core/poller.h"
#include "core/socket.h"
#include "core/thread_pool.h"
#include "core/utils.h"

#ifndef NEXT_SERVER_H_
#define NEXT_SERVER_H_

namespace Next {
class NextServer {
public:
  NextServer(NetAddress server_address,
             int concurrency =
                 static_cast<int>(std::thread::hardware_concurrency()) - 1)
      : pool_(std::make_unique<ThreadPool>(concurrency)),
        listener_(std::make_unique<Looper>()) {
    for (size_t i = 0; i < pool_->GetSize(); i++) {
      reactors_.push_back(std::make_unique<Looper>(TIMER_EXPIRATION))
    }
    for (auto &reactor : reactors_) {
      pool_->SubmitTask([capture0 = reactor.get()]() { capture0->Loop(); });
    }
    std::vector<Looper *> raw_reactors;
    raw_reactors.reserve(reactors_.size());

    std::transform(reactors_.begin(), reactors_.end(),
                   std::back_inserter(raw_reactors),
                   [](auto &uni_ptr) { return uni_ptr.get(); });
    acceptor_ = std::make_unique<Acceptor>(listener_.get(), raw_reactors,
                                           server_address);
  }

  ~NextServer() = default;

  auto OnAccept(std::function<void(Connection *)> on_accept) -> NextServer & {
    acceptor_->SetCustomAcceptCallback(std::move(on_accept));
    return *this;
  }

  auto OnHandle(std::function<void(Connection *)> on_handle) -> NextServer & {
    acceptor_->SetCustomHandleCallback(std::move(on_handle));
    on_handle_set_ = true;
    return *this;
  }

  void Begin() {
    if (!on_handle_set_) {
      throw std::logic_error(
          "Please specify OnHandle callback function before starts");
    }
    listener_->Loop();
  }

private:
  bool on_handle_set_{false};
  std::unique_ptr<Acceptor> acceptor_;
  std::vector<std::unique_ptr<Looper>> reactors_;
  std::unique_ptr<ThreadPool> pool_;
  std::unique_ptr<Looper> listener_;
}
} // namespace Next
#endif