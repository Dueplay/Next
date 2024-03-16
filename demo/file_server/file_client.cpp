
#include <unistd.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include "core/connection.h"
#include "core/net_addr.h"
#include "core/socket.h"
#include "core/thread_pool.h"
#include "http/http_utils.h"

#define BUF_SIZE 2048

namespace Next {
class File_Client {
 public:
  explicit File_Client(NetAddress &server_address) {
    auto client_socket = std::make_unique<Socket>();
    client_socket->Connect(server_address);
    client_conn = std::make_unique<Connection>(std::move(client_socket));
    std::cout << "File_Client: Connected with " << server_address.ToString() << std::endl;
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
      auto tokens = Next::Http::Spilt(buf, "\n");
      if (tokens.empty()) {
        std::cout << "input error, try again." << std::endl;
        continue;
      }
      std::string filename = tokens[0];
      std::ifstream file(filename, std::ios::binary | std::ios::ate);
      if (!file) {
        std::cout << "can't open file, try again." << std::endl;
        continue;
      }
      int file_size = file.tellg();
      file.seekg(0, std::ios::beg);

      int send_size = 0;
      // std::cout << "file :" << filename << ", size : " << file_size << std::endl;
      while (send_size < file_size) {
        memset(buf, 0, sizeof(buf));
        int bufferSize = (file_size - send_size) < BUFSIZ + 1 ? (file_size - send_size) : BUFSIZ + 1;
        file.read(buf, bufferSize);
        send(fd, buf, bufferSize, 0);
        send_size += bufferSize;
      }
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
              << "\tfilename\n"
              << "\tquit(q)\n\n";
  }

 private:
  std::unique_ptr<Connection> client_conn;
};

}  // namespace Next

int main(int argc, char *argv[]) {
  Next::NetAddress server_addr{"0.0.0.0", 10080};
  Next::File_Client dict_client{server_addr};
  dict_client.Begin();
  return 0;
}