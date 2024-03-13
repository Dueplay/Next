#include <map>
#include <set>
#include <string>
#include <vector>
#include "core/next_server.h"
#include "http/http_utils.h"

static const std::set<std::string> VALID_METHOD = {"GET", "SET", "DEL"};
static std::map<std::string, std::string> DICT_STORE;
static const std::string ERR_MSG = "Command Error: [GET,SET,DEL]\n";
static const std::string OK_MSG = "OK.\n";
static const std::string NULL_MSG = "(nil)\n";
static const std::string NEW_LINE = "\n";

auto ProcessQuery(const std::string &query) -> std::string {
  using Next::Http::Spilt;
  using Next::Http::Trim;
  auto tokens = Spilt(query);
  if (tokens.empty() || VALID_METHOD.find(tokens[0]) == VALID_METHOD.end()) {
    return ERR_MSG;
  }
  std::string method = tokens[0];
  if (method == "GET") {
    if (tokens.size() != 2) {
      return "Argument Length Error: GET [key]\n";
    }
    std::string key = Trim(tokens[1], "\n");
    if (DICT_STORE.find(key) == DICT_STORE.end()) {
      return NULL_MSG;
    }
    return DICT_STORE[key] + "\n";
  }

  if (method == "SET") {
    if (tokens.size() != 3) {
      return "Argument Length Error: SET [key] [val]\n";
    }
    std::string val = Trim(tokens[2], "\n");
    DICT_STORE[tokens[1]] = val;
    return OK_MSG;
  }

  if (method == "DEL") {
    if (tokens.size() != 2) {
      return "Argument Length Error: DEL [key]\n";
    }
    std::string key = Trim(tokens[1], "\n");
    if (DICT_STORE.find(key) == DICT_STORE.end()) {
      return NULL_MSG;
    }
    DICT_STORE.erase(key);
    return OK_MSG;
  }
  return "Error: Unknown Command " + method + "\n";
}

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
          auto optional_query = client_conn->FindAndPopTill("\n");
          if (optional_query.has_value()) {
            client_conn->WriteToWriteBuffer(ProcessQuery(optional_query.value()));
            client_conn->Send();
          }
        }
      })
      .Begin();
  return 0;
}