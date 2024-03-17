#ifndef NEXT_SOCKET_H
#define NEXT_SOCKET_H

#include "core/net_addr.h"
#include "core/utils.h"

namespace Next {

class NetAddress;

enum class Protocol;

class Socket {
 public:
  Socket() noexcept = default;

  explicit Socket(int fd) noexcept;

  NON_COPYABLE(Socket);

  // 移动构造
  Socket(Socket &&other) noexcept;

  auto operator=(Socket &&other) noexcept -> Socket &;

  ~Socket();

  auto GetFd() const noexcept -> int;

  // 用于客户端，一步直接连接
  void Connect(NetAddress &server_addr);

  // 用于服务端，三部，bind + listen + accept
  void Bind(NetAddress &server_addr, bool set_reusable = true);

  void Listen();

  auto Accept(NetAddress &client_addr) -> int;

  void SetReusable();

  void SetNonBlocking();

  auto GetAttr() -> int;

 private:
  void CreateByProtocol(Protocol protocol);
  int fd_{-1};
};

}  // namespace Next
#endif  // !NEXT_SOCKET_H