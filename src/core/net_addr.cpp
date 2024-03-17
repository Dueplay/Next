#include "core/net_addr.h"

namespace Next {
NetAddress::NetAddress() noexcept {
  memset(&addr_, 0, sizeof(addr_));
  addr_len_ = sizeof(addr_);
}
NetAddress::NetAddress(const char *ip, uint16_t port, Protocol protocol) : protocol_(protocol) {
  memset(&addr_, 0, sizeof(addr_));
  addr_len_ = sizeof(addr_);
  if (protocol_ == Protocol::Ipv4) {
    auto sockaddr_in4_ = reinterpret_cast<struct sockaddr_in *>(&addr_);
    sockaddr_in4_->sin_family = AF_INET;
    inet_pton(AF_INET, ip, &sockaddr_in4_->sin_addr.s_addr);
    sockaddr_in4_->sin_port = htons(port);
  } else {
    auto sockaddr_in6_ = reinterpret_cast<struct sockaddr_in6 *>(&addr_);
    sockaddr_in6_->sin6_family = AF_INET6;
    inet_pton(AF_INET6, ip, &sockaddr_in6_->sin6_addr.s6_addr);
    sockaddr_in6_->sin6_port = htons(port);
  }
}

auto NetAddress::ToSockaddr() -> struct sockaddr * { return reinterpret_cast<struct sockaddr *>(&addr_); }

auto NetAddress::getSocklen() -> socklen_t * { return &addr_len_; }

auto NetAddress::GetProtocol() const noexcept -> Protocol { return protocol_; }

auto NetAddress::GetIp() const noexcept -> std::string {
  char ip_address[INET6_ADDRSTRLEN];
  if (protocol_ == Protocol::Ipv4) {
    auto sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>(&addr_);
    inet_ntop(AF_INET, &sockaddr_ipv4->sin_addr, ip_address, INET_ADDRSTRLEN);
  } else {
    auto sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>(&addr_);
    inet_ntop(AF_INET6, &sockaddr_ipv6->sin6_addr, ip_address, INET6_ADDRSTRLEN);
  }
  return ip_address;
}

auto NetAddress::GetPort() const noexcept -> uint16_t {
  uint16_t port;
  if (protocol_ == Protocol::Ipv4) {
    auto sockaddr_ipv4 = reinterpret_cast<struct sockaddr_in *>(&addr_);
    port = ntohs(sockaddr_ipv4->sin_port);
  } else {
    auto sockaddr_ipv6 = reinterpret_cast<struct sockaddr_in6 *>(&addr_);
    port = ntohs(sockaddr_ipv6->sin6_port);
  }
  return port;
}
auto NetAddress::ToString() const noexcept -> std::string {
  return GetIp() + std::string(" @ ") + std::to_string(GetPort());
}

auto operator<<(std::ostream &os, const NetAddress &address) -> std::ostream & {
  os << address.ToString();
  return os;
}

}  // namespace Next
