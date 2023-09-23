#include "core/socket.h"
#include <sys/socket.h>
#include <cassert>
#include <fcntl.h>
#include <unistd.h>

namespace Next{

static constexpr int BACK_LOG = 128;

Socket::Socket(int fd) noexcept : fd_(fd){}

Socket::Socket(Socket&& other) noexcept {
    fd_ = other.fd_;
    other.fd_ = -1;
}

auto Socket::operator=(Socket&& other) noexcept -> Socket & {
    if (fd_ != -1) {
        close(fd_);
    }
    std::swap(other.fd_, fd_);
    return *this;

}

Socket::~Socket() {
    if(fd_ != -1) {
        close(fd_);
        fd_ = -1;
    }
}
auto Socket::GetFd() const noexcept -> int { return fd_; }

void Socket::Connect(NetAddress& server_addr) {
    if (fd_ == -1) {
        CreateByProtocol(server_addr.getProtocol());
    }
    if (connect(fd_, server_addr.ToSockaddr(), server_addr.getSocklen()) == -1) {\
        // log error
    }
}

void Socket::Bind(NetAddress &server_addr, SocketFlag flag) {
    if (fd_ == -1){
        // fist create fd
        CreateByProtocol(server_addr.getProtocol());
    }
    if (flag == SocketFlag::set_reusable){
        // 设置端口复用
        SetReusable();
    }
    if (bind(fd_,server_addr.ToSockaddr(),*server_addr.getSocklen()) == -1){
        // 错误日志
    }
}

void Socket::Listen(){
    assert(fd_ != -1 && "cannot Listen with invalid fd");
    if (listen(fd_, BACK_LOG) == -1) {
        // error
    }
}

auto Socket::Accept(NetAddress& client_addr) -> int{
    assert(fd_ != -1 && "cannot Accept with invaild fd");
    int clien_fd = accept (fd_, client_addr.ToSockaddr(), client_addr.getSocklen());
    if(clien_fd == -1) {
        // 高压力下，accept可能会失败，但服务器不能因为这个失败终止掉。
        // log 

    }
    return clien_fd;
}

void Socket::SetReusable() {
    assert(fd_ != -1 && "cannot SetReusable with invaild fd");
    int yes = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1 ||
        setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &yes, sizeof yes) == -1) {
        // log error
    }
}

void Socket::SetNonBlocking() {
    assert(fd_ != -1 && "cannot SetNonBlocking with invaild fd");
    if (fcntl(fd_, F_SETFL, fcntl(fd_, F_GETFL) | O_NONBLOCK) == -1) {
        // log error
    }
}


void Socket::CreateByProtocol(Protocol protocol)
{
    if (protocol == Protocol::Ipv4){
        fd_ = socket(AF_INET, SOCK_STREAM, 0);
    } else {
        fd_ = socket(AF_INET6,SOCK_STREAM,0);
    }
    if (fd_ == -1) {
        // error
    }
}

auto Socket::GetAttr() -> int {
    assert(fd_ != -1 && "cannot GetAttr with invaild fd");
    return fcntl(fd_, F_GETFL);
}

} // namespace Next

