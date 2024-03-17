/**
 * This is the unit test file for core/Looper class
 */

#include "core/looper.h"

#include <unistd.h>

#include <atomic>
#include <memory>
#include <numeric>
#include <thread>  // NOLINT
#include <vector>

#include "catch2/catch_test_macros.hpp"
#include "core/connection.h"
#include "core/net_addr.h"
#include "core/poller.h"
#include "core/socket.h"

/* for convenience reason */
using Next::Connection;
using Next::Looper;
using Next::NetAddress;
using Next::POLL_ADD;
using Next::POLL_ET;
using Next::POLL_READ;
using Next::Poller;
using Next::Socket;

TEST_CASE("[core/looper]") {
  Looper looper;
  // build the server socket
  NetAddress local_host("127.0.0.1", 20080);
  Socket server_sock;
  server_sock.Bind(local_host);
  server_sock.Listen();
  REQUIRE(server_sock.GetFd() != -1);  // 断言。与check的区别是：require断言失败终止测试，而check断言失败继续测试。

  SECTION("looper execute connection's callback func when triggered") {
    std::atomic<int> var = 0;
    int client_num = 3;
    std::vector<std::thread> threads;
    for (int i = 0; i < client_num; i++) {
      threads.emplace_back([&host = local_host]() {
        auto client_socket = Socket();
        client_socket.Connect(host);
        sleep(1);
      });
    }

    // build 3 connections and add into looper with customized callback function
    std::vector<int> reach(client_num, 0);
    for (int i = 0; i < client_num; i++) {
      NetAddress client_address;
      auto client_sock = std::make_unique<Socket>(server_sock.Accept(client_address));
      CHECK(client_sock->GetFd() != -1);
      client_sock->SetNonBlocking();
      auto client_conn = std::make_unique<Connection>(std::move(client_sock));
      client_conn->SetEvents(POLL_READ | POLL_ET);
      // 客户端没有发送数据，回调是在断开连接时触发的。
      // 如果客户端主动关闭连接（例如，通过发送FIN包），服务器端的epoll机制也会报告一个POLL_READ事件，因为这种情况下尝试读取数据会发现已经到达流的末尾（EOF），即读操作返回0。
      client_conn->SetCallback([&reach = reach, index = i](Connection *conn) {
        std::cout << index << " callback exec" << std::endl;
        reach[index] = 1;
      });
      looper.AddConnection(std::move(client_conn));
    }

    /* the looper execute each client's callback once, upon their exit */
    std::thread runner([&]() { looper.Loop(); });
    sleep(2);
    looper.Exit();

    /* each client's callback should have already been executed */
    CHECK(std::accumulate(reach.begin(), reach.end(), 0) == client_num);

    runner.join();
    for (int i = 0; i < client_num; i++) {
      threads[i].join();
    }
  }
}
