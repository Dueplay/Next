#ifndef NEXT_REQUEST_H
#define NEXT_REQUEST_H

#include "core/utils.h"
#include "http/header.h"
#include <string>
#include <vector>
namespace Next::Http {
class Header;
enum class Method;
enum class Version;

/**
 * 仅限 GET/HEAD请求，HTTP
 * 请求类包含必要的请求行功能，包括方法、http 版本、资源 url，并且由于我们支持
 * http 1.1，它还关心客户端连接是否应保持活动状态
 */
class Request {
public:
  Request(Method method, std::string resource_url, Version version,
          const std::vector<Header> &heads) noexcept;
  explicit Request(const std::string &request_str) noexcept;
  NON_COPYABLE(Request);
  auto IsValid() const noexcept -> bool;
  auto ShouldClose() const noexcept -> bool;
  auto GetInvalidReason() const noexcept -> std::string;
  auto GetMethod() const noexcept -> Method;
  auto GetVersion() const noexcept -> Version;
  auto GetResourceUrl() const noexcept -> std::string;
  auto GetHeaders() const noexcept -> std::vector<Header>;
  friend auto operator<<(std::ostream &os, const Request &request)
      -> std::ostream &;

private:
  auto ParseRequestLine(const std::string &request_line) -> bool;
  void ScanHeader(const Header &header);
  // htpp请求格式：
  // 请求行： 包括方法（如GET、POST等）、请求的资源路径（URL）和HTTP版本。
  // 例如:GET /index.html HTTP/1.1
  // 请求头： kv，包含请求的元数据，如客户端能接受的内容类型（Accept）、
  // 使用的编码（Content-Type）和浏览器的类型（User-Agent）等
  // 空行
  // 请求体：不是所有请求都有体部分，主要用于POST和PUT请求，包含发送的数据。
  Method method_;
  std::string resource_url_;
  Version version_;
  std::vector<Header> headers_;
  bool should_close_{true};
  bool is_valid_{false};
  std::string invalid_reason_;
};
} // namespace Next::Http

#endif