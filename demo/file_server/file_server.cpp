#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
#include "core/next_server.h"
#include "http/http_utils.h"

#define BUF_SIZE 2048

int main(int argc, char *argv[]) {
  Next::NetAddress address{"0.0.0.0", 10080};
  Next::NextServer server(address);
  server
      .OnHandle([&](Next::Connection *client_conn) {
        auto [read, exit] = client_conn->Recv();
        if (exit) {
          client_conn->GetLooper()->DeleteConnection(client_conn->GetFd());
          // client_conn ptr invalid, not use in below.
          return;
        }
        if (read) {
          char buffer[BUF_SIZE + 1] = {0};
          const char *message = "File received\n";

          std::ofstream outFile("./ReceivedFile.txt", std::ios::binary);
          if (!outFile.is_open()) {
            std::cerr << "Failed to open file for writing.\n";
            client_conn->GetLooper()->DeleteConnection(client_conn->GetFd());
            return;
          }
          // std::cout << client_conn->GetReadBufferSize() << std::endl;
          // std::cout << client_conn->ReadAsString().c_str() << std::endl;

          outFile.write(client_conn->ReadAsString().c_str(), read);

          client_conn->WriteToWriteBuffer(message);
          client_conn->Send();
          client_conn->ClearReadBuffer();
          outFile.close();
        }
      })
      .Begin();
  return 0;
}