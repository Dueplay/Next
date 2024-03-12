#include "http/response.h"
#include "http/header.h"
#include "http/http_utils.h"
#include <algorithm>
#include <sstream>
#include <utility>

namespace Next::Http {
Response::Response(const std::string &status_code, bool should_close,
                   std::optional<std::string> resource_url)
    : should_close_(should_close), resource_url_(std::move(resource_url)) {
  std::stringstream str_stream;
  str_stream << HTTP_VERSION << SPACE << status_code;
  status_line_ = str_stream.str();
  // 添加一些必要的header
  headers_.emplace_back(HEADER_SERVER, SERVER_NEXT);
  headers_.emplace_back(
      HEADER_CONNECTION,
      ((should_close_) ? CONNECTION_CLOSE : CONNECTION_KEEP_ALIVE));
  // 响应体Body
  if (resource_url_.has_value() && IsFileExists(resource_url_.value())) {
    size_t content_length = CheckFileSize(resource_url_.value());
    headers_.emplace_back(HEADER_CONTENT_LENGTH,
                          std::to_string(content_length));
    // 解析拓展名
    auto last_dot = resource_url_.value().find_last_of(DOT);
    if (last_dot != std::string::npos) {
      auto extension_raw_str = resource_url_.value().substr(last_dot + 1);
      auto extension = ToExtension(extension_raw_str);
      headers_.emplace_back(HEADER_CONTENT_TYPE, ExtensionToMime(extension));
    }
  } else {
    resource_url_ = std::nullopt;
    headers_.emplace_back(HEADER_CONTENT_LENGTH, CONTENT_LENGTH_ZERO);
  }
}

auto Response::Make200Response(bool should_close,
                               std::optional<std::string> resource_url)
    -> Response {
  return {RESPONSE_OK, should_close, std::move(resource_url)};
}
auto Response::Make400Response() noexcept -> Response {
  return {RESPONSE_BAD_REQUEST, true, std::nullopt};
}
auto Response::Make404Response() noexcept -> Response {
  return {RESPONSE_NOT_FOUND, true, std::nullopt};
}
auto Response::Make503Response() noexcept -> Response {
  return {RESPONSE_SERVICE_UNAVAILABLE, true, std::nullopt};
}

void Response::Serialize(std::vector<unsigned char> &buffer) {
  std::stringstream str_stream;
  str_stream << status_line_ << CRLF;
  for (const auto &header : headers_) {
    str_stream << header.Serialize();
  }
  str_stream << CRLF;
  std::string response_head = str_stream.str();
  buffer.insert(buffer.end(), response_head.begin(), response_head.end());
}

auto Response::GetHeaders() -> std::vector<Header> { return headers_; }

auto Response::ChangeHeader(const std::string &key,
                            const std::string &new_value) noexcept -> bool {
  for (auto &it : headers_) {
    if (it.GetKey() == key) {
      it.SetValue(new_value);
      return true;
    }
  }
  return false;
}

auto operator<<(std::ostream &os, Response &response) -> std::ostream & {
  os << "Status_line_: " << response.status_line_ << std::endl;
  os << "Connection Keep Alive: "
     << ((response.should_close_) ? "False" : "True") << std::endl;
  os << "Headers: " << std::endl;
  auto headers = response.GetHeaders();
  std::for_each(headers.begin(), headers.end(),
                [&](auto header) { os << header.Serialize(); });

  return os;
}
} // namespace Next::Http