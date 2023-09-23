
#ifndef NEXT_NET_ADDR_H
#define NEXT_NET_ADDR_H

#include <arpa/inet.h>
#include <string>
#include <cstring>
#include <iostream>
namespace Next{

enum class SocketFlag { set_reusable };
enum class Protocol { Ipv4,Ipv6} ;
class NetAddress{
public:
    NetAddress() noexcept;
    NetAddress(const char *ip, uint16_t port, Protocol protocol = Protocol::Ipv4);
    ~NetAddress() = default;
    auto ToSockaddr() -> struct sockaddr*;
    auto getSocklen() -> socklen_t*;
    auto getProtocol() const noexcept -> Protocol;
    auto GetIp() const noexcept -> std::string;
    auto GetPort() const noexcept -> uint16_t;
    auto ToString() const noexcept -> std::string;
    auto GetSockaddr_in() -> struct sockaddr_in*{ return reinterpret_cast<struct sockaddr_in*>(&addr_); };
    auto GetSockaddr_in6() -> struct sockaddr_in6*{ return reinterpret_cast<struct sockaddr_in6*>(&addr_); };
private:
    Protocol protocol_{Protocol::Ipv4};
    mutable struct sockaddr_storage addr_ {};
    socklen_t addr_len_;
};

auto operator<<(std::ostream& os, const NetAddress& address) -> std::ostream &;

} // namespace Next

#endif // !NEXT_NET_ADDR_H
