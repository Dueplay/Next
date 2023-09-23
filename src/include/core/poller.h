#ifndef NEXT_POLLER_H
#define NEXT_POLLER_H

#include <sys/epoll.h>
#include <memory>
#include <vector>

#include "core/utils.h"
namespace Next {

static constexpr int DEFAULT_EVENTS_LISTEND = 1024;

static constexpr unsigned POLL_ADD = EPOLL_CTL_ADD;
static constexpr unsigned POLL_READ = EPOLLIN;
static constexpr unsigned POLL_ET = EPOLLET;


class Connection;

class Poller {
public:
    explicit Poller(uint64_t epoll_size = DEFAULT_EVENTS_LISTEND);
    
    ~Poller();
    
    NON_COPYABLE(Poller);

    void AddConnection(Connection *conn);

    auto Poll(int timeout = -1) -> std::vector<Connection *>;

    auto GetPollSize() const noexcept -> uint64_t;

private:
    int epoll_fd_;
    uint64_t epoll_size_;
    struct epoll_event *epoll_events_{nullptr};

};
} // end of namespace next
#endif // !NEXT_POLLER_H