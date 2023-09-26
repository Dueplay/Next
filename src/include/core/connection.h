#ifndef NEXT_CONNECTION_H
#define NEXT_CONNECTION_H
#include "core/buffer.h"
#include "core/looper.h"
#include "core/socket.h"
#include "core/utils.h"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
namespace Next {

static const int TEMP_BUF_SIZE = 2048;
/**
 * 该Connection类封装了一个TCP客户端连接
 * 可设置新消息到达时自定义回调函数
 * 包含监听事件和返回事件的信息
 * 这样Poller就可以基于这个Connection类进行操作和epoll
 *
 *
 */

class Connection {
public:
  explicit Connection(std::unique_ptr<Socket> socket);
  ~Connection() = default;

  NON_COPYABLE(Connection);

  auto GetFd() const noexcept -> int;
  auto GetSocket() noexcept -> Socket *;

  void SetEvents(uint32_t events);
  auto GetEvents() const noexcept -> uint32_t;
  void SetRevents(uint32_t revents);
  auto GetRevents() const noexcept -> uint32_t;

  void SetCallback(std::function<void(Connection *)> callback);
  auto GetCallback() noexcept -> std::function<void()>;

  /* for Buffer */
  auto FindAndPopTill(const std::string &target) -> std::optional<std::string>;
  auto GetReadBufferSize() const noexcept -> size_t;
  auto GetWriteBufferSize() const noexcept -> size_t;
  void WriteToReadBuffer(const unsigned char *buf, size_t size);
  void WriteToWriteBuffer(const unsigned char *buf, size_t size);
  void WriteToReadBuffer(const std::string &str);
  void WriteToWriteBuffer(const std::string &str);
  void WriteToWriteBuffer(std::vector<unsigned char> &&other_buf);

  auto Read() const noexcept -> const unsigned char *;
  auto ReadAsString() const noexcept -> std::string;

  /* return std::pair<How many bytes read, whether the client exits> */
  auto Recv() -> std::pair<ssize_t, bool>;
  void Send();
  void ClearReadBuffer() noexcept;
  void ClearWriteBuffer() noexcept;
  void SetLooper(Looper *looper) noexcept;
  auto GetLooper() noexcept -> Looper *;

private:
  Looper *owner_looper_{nullptr};
  std::unique_ptr<Socket> socket_;
  std::unique_ptr<Buffer> read_buffer_;
  std::unique_ptr<Buffer> write_buffer_;
  uint32_t events_{0};
  uint32_t revents_{0};
  std::function<void()> callback_{nullptr};
};

} // namespace Next

#endif // !NEXT_CONNECTION_H