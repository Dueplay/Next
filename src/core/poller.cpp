#include "core/poller.h"
#include <cstring>
#include <unistd.h>
#include <cassert>
#include "core/connection.h"
namespace Next {

Poller::Poller(uint64_t epoll_size) : epoll_size_(epoll_size) {
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1) {
        perror("Poller: epoll_create1() error");
        exit(EXIT_FAILURE);
    }
    epoll_events_ = new struct epoll_event[epoll_size_];
    memset(epoll_events_, 0, epoll_size_ * sizeof(struct epoll_event));
}
    
Poller::~Poller() {
    if (epoll_fd_ != -1) {
        close(epoll_fd_);
        delete[] epoll_events_;
        epoll_fd_ = -1;
    }
}
    

void Poller::AddConnection(Connection *conn) {
    assert(conn->GetFd() != -1 && "cannot AddConnection() with an invalid fd");
    struct epoll_event event;
    memset(&event, 0 ,sizeof(struct epoll_event));
    event.events = conn->GetEvents();
    event.data.ptr = conn;
    
    int ret = epoll_ctl(epoll_fd_, POLL_ADD, conn->GetFd(), &event);
    if (ret == -1) {
        perror("Poller: epoll_ctl add error");
        exit(EXIT_FAILURE);
    }
}

auto Poller::Poll(int timeout) -> std::vector<Connection *> {
    std::vector<Connection *> events_happen;
    int ready = epoll_wait(epoll_fd_, epoll_events_, epoll_size_, timeout);
    if (ready == -1) {
        perror("Poller: Poll() error");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < ready; i++) {
        Connection * ready_conn = reinterpret_cast<Connection *>(epoll_events_[i].data.ptr);
        ready_conn->SetEvents(epoll_events_[i].events);
        events_happen.emplace_back(ready_conn);
    }
    return events_happen;
}

auto Poller::GetPollSize() const noexcept -> uint64_t { return epoll_size_; }

}
