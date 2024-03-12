#include "core/next_server.h"
#include "http/cgier.h"
#include "http/header.h"
#include "http/http_utils.h"
#include "http/request.h"
#include "http/response.h"
#include "log/logger.h"

namespace Next::Http {

void PrecessHttpRequest(const std::string &serving_dir,
                        std::shared_ptr<Cache> &cache,
                        Connection *client_conn) {
  // ET模式，一次性读完所有数据
  int from_fd = client_conn->GetFd();
  auto [read, exit] = client_conn->Recv();
  if (exit) {
    client_conn->GetLooper()->DeleteConnection(from_fd);
    // client_conn指针被释放，不应该再访问
    LOG_INFO("client fd=" + std::to_string(from_fd) + "has exited");
    return;
  }
  // 检察是否有http请求
  bool no_more_parse = false;
  std::optional<std::string> request_op =
      client_conn->FindAndPopTill("\r\n\r\n");
  while (request_op != std::nullopt) {
    Request request{request_op.value()};
    std::vector<unsigned char> response_buf;
    if (!request.IsValid()) {
      auto response = Response::Make400Response();
      response.Serialize(response_buf);
      no_more_parse = true;
    } else {
      std::string resource_full_path = serving_dir + request.GetResourceUrl();
      std::cout << "recv a http request, request path: " << resource_full_path
                << std::endl;
      if (IsCgiRequest(resource_full_path)) {
        // dynamic cgi request
        Cgier cgier = Cgier::ParseCgier(resource_full_path);
        if (!cgier.IsValid()) {
          auto response = Response::Make400Response();
          response.Serialize(response_buf);
          no_more_parse = true;
        } else {
          auto cgi_program = cgier.GetPath();
          if (!IsFileExists(cgi_program)) {
            auto response = Response::Make404Response();
            response.Serialize(response_buf);
            no_more_parse = true;
          } else {
            auto cgi_result = cgier.Run();
            auto response =
                Response::Make200Response(request.ShouldClose(), std::nullopt);
            response.ChangeHeader(HEADER_CONTENT_LENGTH,
                                  std::to_string(cgi_result.size()));
            no_more_parse = request.ShouldClose();
            response.Serialize(response_buf);
            // 加上响应体为cgi程序结果
            response_buf.insert(response_buf.end(), cgi_result.begin(),
                                cgi_result.end());
          }
        }
      } else {
        // http static resourse request
        if (!IsFileExists(resource_full_path)) {
          auto response = Response::Make404Response();
          response.Serialize(response_buf);
          no_more_parse = true;
        } else {
          auto response = Response::Make200Response(request.ShouldClose(),
                                                    resource_full_path);
          response.Serialize(response_buf);
          no_more_parse = request.ShouldClose();
          std::vector<unsigned char> cache_buf;
          if (request.GetMethod() == Method::GET) {
            bool resource_cached =
                cache->TryLoad(resource_full_path, cache_buf);
            if (!resource_cached) {
              LoadFile(resource_full_path, cache_buf);
              cache->TryInsert(resource_full_path, cache_buf);
            }
          }
          response_buf.insert(response_buf.end(), cache_buf.begin(),
                              cache_buf.end());
        }
      }
    }
    // 发送响应
    client_conn->WriteToWriteBuffer(std::move(response_buf));
    client_conn->Send();
    if (no_more_parse) {
      break;
    }
    request_op = client_conn->FindAndPopTill("\r\n\r\n");
  }

  if (no_more_parse) {
    client_conn->GetLooper()->DeleteConnection(from_fd);
    // client_conn指针被释放，不应该再访问
    return;
  }
}
} // namespace Next::Http

int main(int argc, char *argv[]) {
  const std::string usage =
      "Usage: \n"
      "./http_server [optional: port default=20080] [optional: directory "
      "default=../http_dir/] \n";
  if (argc > 3) {
    std::cout << "argument number error\n";
    std::cout << usage;
    exit(EXIT_FAILURE);
  }
  Next::NetAddress address("0.0.0.0", 20080);
  std::string dir = "../http_dir/";
  if (argc >= 2) {
    auto port = static_cast<uint16_t>(std::strtol(argv[1], nullptr, 10));
    if (port == 0) {
      std::cout << "port error\n";
      std::cout << usage;
      exit(EXIT_FAILURE);
    }
    address = {"0.0.0.0", port};
    if (argc == 3) {
      dir = argv[2];
      if (!Next::Http::IsDirectoryExists(dir)) {
        std::cout << "directory error\n";
        std::cout << usage;
        exit(EXIT_FAILURE);
      }
    }
  }
  Next::NextServer server(address);
  auto cache = std::make_shared<Next::Cache>();
  server
      .OnHandle([&](Next::Connection *client_conn) {
        Next::Http::PrecessHttpRequest(dir, cache, client_conn);
      })
      .Begin();
  return 0;
}