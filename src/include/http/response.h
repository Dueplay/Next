#ifndef NEXT_RESPONSE_H
#define NEXT_RESPONSE_H

#include <optional>
#include <string>
#include <vector>

namespace Next::Http {
class Header;
class Response {
public:
  Response(const std::string &status_code, bool should_close,
           std::optional<std::string> resource_url);

  /* 200 OK response */
  static auto Make200Response(bool should_close,
                              std::optional<std::string> resource_url)
      -> Response;
  /* 400 Bad Request response, close connection */
  static auto Make400Response() noexcept -> Response;
  /* 404 Not Found response, close connection */
  static auto Make404Response() noexcept -> Response;
  /* 503 Service Unavailable response, close connection */
  static auto Make503Response() noexcept -> Response;

  /* no content, content should separately be loaded */
  void Serialize(std::vector<unsigned char> &buffer); // NOLINT

  auto GetHeaders() -> std::vector<Header>;

  auto ChangeHeader(const std::string &key,
                    const std::string &new_value) noexcept -> bool;

  friend auto operator<<(std::ostream &os, Response &response)
      -> std::ostream &;

private:
  // http响应格式
  /**
   * 状态行：包括HTTP版本、状态码（如200、404等）和状态消息（如OK、Not
   * Found等）。例如: HTTP/1.1 200 OK
   * 响应头（Headers）：包含响应的元数据，如内容类型（Content-Type）、内容长度（Content-Length）和服务器信息（Server）等。
   * 响应体（Body）：包含服务器返回的实际数据，比如HTML文档、图片或其他类型的文件。
   */
  std::string status_line_;
  bool should_close_;
  std::vector<Header> headers_;
  std::optional<std::string> resource_url_;
  std::vector<unsigned char> body_;
};
} // namespace Next::Http
#endif