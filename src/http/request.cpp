#include "http/request.h"
#include "http/http_utils.h"
#include <algorithm>

namespace Next::Http {
  
Request::Request(Method method, std::string resource_url, Version version,
                 const std::vector<Header> &heads) noexcept
    : method_(method), resource_url_(std::move(resource_url)),
      version_(version), headers_(heads), is_valid_(true) {}

Request::Request(const std::string &request_str) noexcept {
  auto lines = Spilt(request_str, CRLF);
  if (lines.size() < 2 || !lines.back().empty()) {
    invalid_reason_ = "Request format is wrong.";
    return;
  }
  // 请求的结尾应该是 '\r\n\r\n'
  if (!lines.back().empty()) {
    invalid_reason_ = "Ending of the request is not \r\n\r\n";
    return;
  }
  lines.pop_back();
  bool request_line_parse_success = ParseRequestLine(lines[0]);
  if (!request_line_parse_success) {
    return;
  }
  lines.erase(lines.begin());
  // 请求行
  for (const auto &line : lines) {
    Header header{line};
    if (!header.IsValid()) {
      invalid_reason_ = "Fail to parse header line: " + line;
      return;
    }
    ScanHeader(header);
    headers_.push_back(std::move(header));
  }
  is_valid_ = true;
}

auto Request::IsValid() const noexcept -> bool { return is_valid_; }

auto Request::ShouldClose() const noexcept -> bool { return should_close_; }

auto Request::GetInvalidReason() const noexcept -> std::string {
  return invalid_reason_;
}
auto Request::GetMethod() const noexcept -> Method { return method_; }

auto Request::GetVersion() const noexcept -> Version { return version_; }

auto Request::GetResourceUrl() const noexcept -> std::string {
  return resource_url_;
}

auto Request::GetHeaders() const noexcept -> std::vector<Header> {
  return headers_;
}

auto Request::ParseRequestLine(const std::string &request_line) -> bool {
  auto tokens = Spilt(request_line, SPACE);
  if (tokens.size() != 3) {
    invalid_reason_ = "Invalid first request headline: " + request_line;
    return false;
  }
  method_ = ToMethod(tokens[0]);
  if (method_ == Method::UNSUPPORTED) {
    invalid_reason_ = "Unsupported method: " + tokens[0];
    return false;
  }
  version_ = ToVersion(tokens[2]);
  if (version_ == Version::UNSUPPORTED) {
    invalid_reason_ = "Unsupported version: " + tokens[2];
    return false;
  }
  // 默认url为index.html
  resource_url_ =
      (tokens[1].empty() || tokens[1].at(tokens[1].size() - 1) == '/')
          ? tokens[1] + DEFAULT_ROUTE
          : tokens[1];
  return true;
}

void Request::ScanHeader(const Header &header) {
  // 目前仅扫描服务后是否应关闭连接
  auto key = Format(header.GetKey());
  if (key == Format(HEADER_CONNECTION)) {
    auto value = Format(header.GetValue());
    if (value == Format(CONNECTION_KEEP_ALIVE)) {
      should_close_ = false;
    }
  }
}

auto operator<<(std::ostream &os, const Request &request) -> std::ostream & {
  if (!request.IsValid()) {
    os << "Request is not invalid." << std::endl;
    os << "Reason: " << request.invalid_reason_ << std::endl;
  } else {
    os << "Request is valid." << std::endl;
    os << "Method: " << METHOD_TO_STRING.at(request.method_) << std::endl;
    os << "Resource Url: " << request.resource_url_ << std::endl;
    os << "HTTP Version: " << VERSION_TO_STRING.at(request.version_)
       << std::endl;
    os << "Connection Keep Alive: "
       << ((request.should_close_) ? "False" : "True") << std::endl;
    os << "Headers: " << std::endl;
    auto headers = request.GetHeaders();
    for_each(headers.begin(), headers.end(),
             [&](auto header) { os << header.Serialize(); });
  }
  return os;
}
} // namespace Next::Http