#include "http/http_utils.h"
#include <algorithm>
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace Next::Http {

Method ToMethod(const std::string &str) {
  std::string formattedStr = Format(str);
  for (const auto &pair : METHOD_TO_STRING) {
    if (pair.second == formattedStr) {
      return pair.first;
    }
  }
  return Method::UNSUPPORTED;
}

Version ToVersion(const std::string &str) {
  std::string formattedStr = Format(str);
  if (formattedStr == VERSION_TO_STRING.at(Version::HTTP_1_1)) {
    return Version::HTTP_1_1;
  }
  return Version::UNSUPPORTED;
}

Extension ToExtension(const std::string &str) {
  std::string formattedStr = Format(str);
  for (const auto &pair : EXTENSION_TO_STRING) {
    if (pair.second == formattedStr) {
      return pair.first;
    }
  }
  return Extension::UNSUPPORTED;
}

auto ExtensionToMime(const Extension &extension) noexcept -> std::string {
  if (extension == Extension::HTML) {
    return MIME_HTML;
  }
  if (extension == Extension::CSS) {
    return MIME_CSS;
  }
  if (extension == Extension::PNG) {
    return MIME_PNG;
  }
  if (extension == Extension::JPG) {
    return MIME_JPG;
  }
  if (extension == Extension::JPEG) {
    return MIME_JPEG;
  }
  if (extension == Extension::GIF) {
    return MIME_GIF;
  }
  if (extension == Extension::OCTET) {
    return MIME_OCTET;
  }
  return MIME_OCTET;
}

auto Spilt(const std::string &str, const char *delim /* = SPACE*/) noexcept
    -> std::vector<std::string> {
  if (str.empty()) {
    return {};
  }
  std::vector<std::string> tokens;
  size_t curr = 0;
  size_t next;
  size_t delim_len = strlen(delim);

  while ((next = str.find(delim, curr)) != std::string::npos) {
    tokens.emplace_back(str.substr(curr, next - curr));
    curr = next + delim_len;
  }
  if (curr != str.size()) {
    // 最后一个
    tokens.emplace_back(str.substr(curr, str.size() - curr));
  }
  return tokens;
}

auto Join(std::vector<std::string> &tokens,
          const char *delim /* = SPACE*/) noexcept -> std::string {
  if (tokens.empty())
    return {};
  if (tokens.size() == 1)
    return tokens[0];
  std::stringstream ssr;
  for (size_t i = 0; i < tokens.size() - 1; i++) {
    ssr << tokens[i] << delim;
  }
  ssr << tokens[tokens.size() - 1];
  return ssr.str();
}

auto Trim(const std::string &str, const char *delim /* = SPACE*/) noexcept
    -> std::string {
  size_t r_found = str.find_last_not_of(delim); // 逆向找第一个不是delim的位置
  if (r_found == std::string::npos)
    return {};
  size_t l_found = str.find_first_not_of(delim); // 正向找第一个不是delim
  return str.substr(l_found, r_found - l_found + 1);
}

auto ToUpper(std::string str) noexcept -> std::string {
  std::transform(std::begin(str), std::end(str), std::begin(str),
                 [](char c) { return std::toupper(c); });
  return str;
}

auto Format(const std::string &str) noexcept -> std::string {
  return ToUpper(Trim(str));
}

auto IsDirectoryExists(const std::string &directory_path) noexcept -> bool {
  return std::filesystem::is_directory(directory_path);
}

auto IsCgiRequest(const std::string &resource_url) noexcept -> bool {
  return resource_url.find(CGI_BIN) != std::string::npos;
}

auto IsFileExists(const std::string &file_path) noexcept -> bool {
  return std::filesystem::exists(file_path);
}

auto DeleteFile(const std::string &file_path) noexcept -> bool {
  return std::filesystem::remove(file_path);
}

auto CheckFileSize(const std::string &file_path) noexcept -> size_t {
  assert(IsFileExists(file_path));
  return std::filesystem::file_size(file_path);
}

void LoadFile(const std::string &file_path,
              std::vector<unsigned char> &buffer) noexcept {
  size_t file_size = CheckFileSize(file_path);
  size_t buffer_old_size = buffer.size();
  buffer.resize(buffer_old_size + file_size);
  std::ifstream file(file_path);
  assert(file.is_open());
  file.read(reinterpret_cast<char *>(&buffer[buffer_old_size]),
            static_cast<std::streamsize>(file_size));
}
} // namespace Next::Http
