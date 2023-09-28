#include "core/next_server.h"

int main() {
  Next::NetAddress local_address("127.0.0.1", 20080);
  Next::NextServer echo_server(local_address);
  echo_server
      .OnHandle([&](Next::Connection *client_conn) {
        int from_fd = client_conn->GetFd();
        std::pair<size_t, bool> ret = client_conn->Recv();
        if (ret.second) {
          client_conn->GetLooper()->DeleteConnection(from_fd);
          return;
        }
        if (ret.first) {
          client_conn->WriteToWriteBuffer(client_conn->ReadAsString());
          client_conn->Send();
          client_conn->ClearReadBuffer();
        }
      })
      .Begin();
  return 0;
}