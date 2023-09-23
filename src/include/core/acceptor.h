#ifndef NEXT__ACCEPTOR_H
#define NEXT__ACCEPTOR_H

#include <vector>
#include <memory>
#include <functional>
#include "core/utils.h"
namespace Next {

class NetAddress;
class Looper;
class Connection;

class Acceptor{
public:
    Acceptor(Looper *listener, std::vector<Looper *> reactors, NetAddress server_address);
    ~Acceptor() = default;
    NON_COPYABLE(Acceptor);
    void BaseAcceptCallback(Connection *server_conn);
    void BaseHandleCallback(Connection *client_conn);
    void SetCustomAcceptCallback(std::function<void(Connection *)> custom_accept_callback);
    void SetCustomHandleCallback(std::function<void(Connection *)> custom_handle_callback);
    auto GetCustomHandleCallback() const noexcept -> std::function<void(Connection *)>;
    auto GetCustomHandleCallback() const noexcept-> std::function<void(Connection *)>;
    auto GetAcceptorConnection() noexcept -> Connection *;
private:
    std::vector<Looper *> reactors_;
    std::unique_ptr<Connection> acceptor_conn_;
    std::function<void(Connection *)> custom_accept_callback_{};
    std::function<void(Connection *)> custom_handle_callback_{};

};
}
#endif