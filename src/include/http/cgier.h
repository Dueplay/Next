#ifndef NEXT_CGIER_H
#define NEXT_CGIER_H

#include <string>
#include <vector>

namespace Next::Http {

class Cgier {
public:
  explicit Cgier(const std::string &path,
                 const std::vector<std::string> &arguments) noexcept;
  ~Cgier() = default;
  auto IsValid() const noexcept -> bool;
  auto GetPath() const noexcept -> std::string;

  static auto ParseCgier(const std::string &resource_url) noexcept -> Cgier;
  static auto MakeInvalidCgier() noexcept -> Cgier;

  auto Run() -> std::vector<unsigned char>;

private:
  auto BuildArgumentList() -> char **;
  void FreeArgumentList(char **arg_list);
  std::string cgi_program_path_;
  std::vector<std::string> cgi_arguments_;
  bool valid_{true};
};

} // namespace Next::Http

#endif