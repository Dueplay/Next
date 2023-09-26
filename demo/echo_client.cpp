#include <sys/socket.h>
#include <unistd.h>

#include <memory>

#include "core/connection.h"
#include "core/net_addr.h"
#include "core/socket.h"
#include "core/thread_pool.h"

#define BUF_SIZE 2048
namespace Next {
class EchoClient {
public:
  explicit EchoClient(NetAddress server_address) {
    auto client_sock = std::make_unique<Socket>();
    client_sock->Connect(server_address);
    client_connection = std::make_unique<Connection>(std::move(client_sock));
  }

  void Begin() {
    char buf[BUF_SIZE + 1];
    memset(buf, 0, sizeof(buf));
    int fd = client_connection->GetFd();
    while (true) {
      auto actual_read = read(STDIN_FILENO, buf, BUF_SIZE);
      send(fd, buf, actual_read, 0);
      memset(buf, 0, sizeof(buf));
      auto actual_recv = recv(fd, buf, BUF_SIZE, 0);
      write(STDOUT_FILENO, buf, actual_recv);
      memset(buf, 0, sizeof(buf));
    }
  }

private:
  std::unique_ptr<Connection> client_connection;
};
} // namespace Next
int main() {
  Next::NetAddress local_address("127.0.0.1", 20080);
  Next::EchoClient echo_client(local_address);
  echo_client.Begin();
  return 0;
}