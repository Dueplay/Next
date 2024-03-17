#ifndef NEXT_HEADER_H
#define NEXT_HEADER_H

#include <ostream>
#include <string>
#include <vector>
namespace Next::Http {
enum class Method;
enum class Version;

class Header {
 public:
  Header(const std::string &key, const std::string &value);
  Header(std::string &&key, std::string &&value);
  explicit Header(const std::string &line);  // deserialize method
  Header(const Header &other) = default;
  Header(Header &&other) noexcept;
  auto operator=(const Header &other) -> Header & = default;
  auto operator=(Header &&other) -> Header &;
  ~Header() = default;

  auto IsValid() const -> bool;
  auto GetKey() const -> std::string;
  auto GetValue() const -> std::string;
  void SetValue(const std::string &new_value) noexcept;
  auto Serialize() const -> std::string;

  friend auto operator<<(std::ostream &os, const Header &header) -> std::ostream &;

 private:
  std::string key_;
  std::string value_;
  bool valid_{true};
};
}  // namespace Next::Http

#endif