#include "core/acceptor.h"
#include "core/connection.h"
#include "core/looper.h"
#include "core/net_addr.h"
#include "core/poller.h"
#include "core/socket.h"
#include "log/logger.h"

namespace Next {

Acceptor::Acceptor(Looper *listener, std::vector<Looper *> reactors,
                   NetAddress server_address)
    : reactors_(std::move(reactors)) {
  auto acceptor_sock = std::make_unique<Socket>();
  acceptor_sock->Bind(server_address, SocketFlag::set_reusable);
  acceptor_sock->Listen();
  acceptor_conn_ = std::make_unique<Connection>(std::move(acceptor_sock));
  acceptor_conn_->SetEvents(POLL_READ);
  acceptor_conn_->SetLooper(listener);
  listener->AddAcceptor(acceptor_conn_.get());
  SetCustomAcceptCallback([](Connection *) {});
  SetCustomHandleCallback([](Connection *) {});
}

/**
 * accept a connect,and create Connection for client,then add to epoll
 */
void Acceptor::BaseAcceptCallback(Connection *server_conn) {
  NetAddress client_address;
  int accept_fd = server_conn->GetSocket()->Accept(client_address);
  if (accept_fd == -1) {
    return;
  }
  auto client_sock = std::make_unique<Socket>(accept_fd);
  client_sock->SetNonBlocking();
  auto client_conn = std::make_unique<Connection>(std::move(client_sock));
  client_conn->SetEvents(POLL_READ | POLL_ET);
  client_conn->SetCallback(GetCustomHandleCallback());

  int idx = rand() % reactors_.size();
  LOG_INFO("new client fd=" + std::to_string(client_conn->GetFd()) +
           " maps to reactor " + std::to_string(idx));
  client_conn->SetLooper(reactors_[idx]);
  reactors_[idx]->AddConnection(std::move(client_conn));
}

void Acceptor::BaseHandleCallback(Connection *client_conn) {
  int fd = client_conn->GetFd();
  if (client_conn->GetLooper()) {
    client_conn->GetLooper()->RefreshConnection(fd);
  }
}
void Acceptor::SetCustomAcceptCallback(
    std::function<void(Connection *)> custom_accept_callback) {
  custom_accept_callback_ = std::move(custom_accept_callback);
  acceptor_conn_->SetCallback([this](auto &&PH1) {
    BaseAcceptCallback(std::forward<decltype(PH1)>(PH1));
    custom_accept_callback_(std::forward<decltype(PH1)>(PH1));
  });
}

void Acceptor::SetCustomHandleCallback(
    std::function<void(Connection *)> custom_handle_callback) {
  custom_handle_callback_ =
      [this, callback = std::move(custom_handle_callback)](auto &&PH1) {
        BaseHandleCallback(std::forward<decltype(PH1)>(PH1));
        callback(std::forward<decltype(PH1)>(PH1));
      };
}

auto Acceptor::GetCustomAcceptCallback() const noexcept
    -> std::function<void(Connection *)> {
  return custom_accept_callback_;
}

auto Acceptor::GetCustomHandleCallback() const noexcept
    -> std::function<void(Connection *)> {
  return custom_handle_callback_;
}

auto Acceptor::GetAcceptorConnection() noexcept -> Connection * {
  return acceptor_conn_.get();
}
} // namespace Next