#include <unistd.h>
#include <cstring>
#include <iostream>
#include <memory>
#include "core/connection.h"
#include "core/net_addr.h"
#include "core/socket.h"
#include "core/thread_pool.h"

#define BUF_SIZE 2048

namespace Next {
class Dict_Client {
 public:
  explicit Dict_Client(NetAddress &server_address) {
    auto client_socket = std::make_unique<Socket>();
    client_socket->Connect(server_address);
    client_conn = std::make_unique<Connection>(std::move(client_socket));
    std::cout << "DictClient: Connected with " << server_address.ToString() << std::endl;
  }
  void Begin() {
    Usage();
    char buf[BUF_SIZE + 1];
    memset(buf, 0, sizeof(buf));
    int fd = client_conn->GetFd();
    while (true) {
      // scan form user keyboard
      auto actual_read = read(STDIN_FILENO, buf, BUF_SIZE);
      if (strncmp(buf, "quit", strlen("quit")) == 0 || strncmp(buf, "q", strlen("q")) == 0) {
        // client indicates to exit
        exit(0);
      }
      send(fd, buf, actual_read, 0);
      memset(buf, 0, sizeof(buf));
      // blocking wait for server's response
      auto actual_recv = recv(fd, buf, BUF_SIZE, 0);
      write(STDOUT_FILENO, buf, actual_recv);
      // scan next user input
      memset(buf, 0, sizeof(buf));
    }
  }
  void Usage() {
    std::cout << "Usage:\n"
              << "\tGET [key]\n"
              << "\tSET [key] [value]\n"
              << "\tDEL [key]\n"
              << "\tquit(q)\n\n";
  }

 private:
  std::unique_ptr<Connection> client_conn;
};

}  // namespace Next

int main(int argc, char *argv[]) {
  Next::NetAddress server_addr{"0.0.0.0", 10080};
  Next::Dict_Client dict_client{server_addr};
  dict_client.Begin();
  return 0;
}