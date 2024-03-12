#include "http/cgier.h"
#include "http/http_utils.h"
#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <sstream>
#include <sys/types.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>
namespace Next::Http {

Cgier::Cgier(const std::string &path,
             const std::vector<std::string> &arguments) noexcept
    : cgi_program_path_(path), cgi_arguments_(arguments), valid_(true) {}

auto Cgier::IsValid() const noexcept -> bool { return valid_; }

auto Cgier::GetPath() const noexcept -> std::string {
  return cgi_program_path_;
}

auto Cgier::ParseCgier(const std::string &resource_url) noexcept -> Cgier {
  if (resource_url.empty() || !IsCgiRequest(resource_url)) {
    return MakeInvalidCgier();
  }
  auto cgi_pos = resource_url.find(CGI_BIN);
  auto cgi_separator = resource_url.find(PARAMETER_SEPARATOR, cgi_pos);
  auto cgi_path = resource_url.substr(0, cgi_separator);
  auto arguments =
      Spilt(resource_url.substr(cgi_separator + 1), PARAMETER_SEPARATOR);
  return Cgier(cgi_path, arguments);
}

auto Cgier::MakeInvalidCgier() noexcept -> Cgier {
  Cgier invalid_cgier{std::string(), std::vector<std::string>()};
  invalid_cgier.valid_ = false;
  return invalid_cgier;
}

// 创建子进程运行cgi程序，通过父子进程间共享文件来进行结果的传递
auto Cgier::Run() -> std::vector<unsigned char> {
  assert(valid_);
  std::vector<unsigned char> cgi_result;

  std::stringstream ssr;
  ssr << CGI_PREFIX << UNDERSCORE << std::this_thread::get_id() << ".txt";
  std::string shared_file_name = ssr.str();
  int fd = open(shared_file_name.c_str(), O_RDWR | O_APPEND | O_CREAT,
                READ_WRITE_PERMISSION);
  if (fd == -1) {
    std::string error = "fail to create/open the file" + shared_file_name;
    return {error.begin(), error.end()};
  }

  pid_t pid = fork();
  if (pid == -1) {
    std::string error = "fail to fork()" + shared_file_name;
    return {error.begin(), error.end()};
  }

  if (pid == 0) {
    // 子进程
    // 将cgi程序的stdout重定向到文件
    dup2(fd, STDOUT_FILENO);
    close(fd);

    char **cgi_argv = BuildArgumentList();
    // 加载可执行cgi程序
    if (execve(cgi_program_path_.c_str(), cgi_argv, nullptr) < 0) {
      // execve执行失败才能执行到这
      perror("fail to execve()");
      FreeArgumentList(cgi_argv);
      exit(1); // 退出子进程
    }
  } else {
    // 父进程
    close(fd);
    int status;
    // 等待子进程退出，回收子进程
    if (waitpid(pid, &status, 0) == -1) {
      std::string error = "fail to harvest child process by waitpid()";
      return {error.begin(), error.end()};
    }

    // 从共享文件中加载cgi程序的结果
    LoadFile(shared_file_name, cgi_result);
    // 删除文件
    DeleteFile(shared_file_name);
  }
  return cgi_result;
}

// 根据cgi_arguments_构建cgi程序的命令行参数argv[]
auto Cgier::BuildArgumentList() -> char ** {
  if (cgi_arguments_.empty())
    return nullptr;
  char **cgi_argv = (char **)calloc(cgi_arguments_.size() + 2, sizeof(char *));
  cgi_argv[0] = (char *)calloc(cgi_program_path_.size() + 1, sizeof(char));
  memcpy(cgi_argv[0], cgi_program_path_.c_str(), cgi_program_path_.size());
  for (size_t i = 0; i < cgi_arguments_.size(); i++) {
    cgi_argv[i + 1] =
        (char *)calloc(cgi_arguments_[i].size() + 1, sizeof(char));
    memcpy(cgi_argv[i + 1], cgi_arguments_[i].c_str(),
           cgi_arguments_[i].size());
  }
  cgi_argv[cgi_arguments_.size() + 1] = nullptr; // 标明argv[]结束
  return cgi_argv;
}

void Cgier::FreeArgumentList(char **arg_list) {
  if (arg_list) {
    for (size_t i = 0; i < cgi_arguments_.size() + 2; i++) {
      if (arg_list[i])
        free(arg_list[i]);
    }
    free(arg_list);
  }
}

} // namespace Next::Http