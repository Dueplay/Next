#include "http/header.h"
#include "http/http_utils.h"
namespace Next::Http {
Header::Header(const std::string &key, const std::string &value)
    : key_(key), value_(value) {}

Header::Header(std::string &&key, std::string &&value)
    : key_(std::move(key)), value_(std::move(value)) {}

Header::Header(const std::string &line) {
  auto tokens = Spilt(line, COLON);
  if (tokens.size() < 2) {
    valid_ = false;
    return;
  }
  key_ = std::move(tokens[0]);
  tokens.erase(std::begin(tokens));
  std::string value = (tokens.size() == 1) ? tokens[0] : Join(tokens, COLON);
  value_ = std::move(value);
}

Header::Header(Header &&other) noexcept
    : key_(std::move(other.key_)), value_(std::move(other.value_)),
      valid_(other.valid_) {}

auto Header::operator=(Header &&other) -> Header & {
  key_ = std::move(other.key_);
  value_ = std::move(other.value_);
  valid_ = other.valid_;
  return *this;
}

auto Header::IsValid() const -> bool { return valid_; }
auto Header::GetKey() const -> std::string { return key_; }
auto Header::GetValue() const -> std::string { return value_; }
void Header::SetValue(const std::string &new_value) noexcept {
  value_ = new_value;
}
auto Header::Serialize() const -> std::string {
  return key_ + COLON + value_ + CRLF;
}

auto operator<<(std::ostream &os, const Header &header) -> std::ostream & {
  os << "HTTP Header contains:" << std::endl;
  os << "Key: " << header.GetKey() << std::endl;
  os << "Value: " << header.GetValue() << std::endl;
  os << "IsValid: " << ((header.IsValid()) ? "True" : "False") << std::endl;
  os << "Serialize to: " << header.Serialize();
  return os;
}
} // namespace Next::Http