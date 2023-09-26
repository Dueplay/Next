#include "core/connection.h"
#include "log/logger.h"
#include <sys/socket.h>
namespace Next {
Connection::Connection(std::unique_ptr<Socket> socket)
    : socket_(std::move(socket)), read_buffer_(std::make_unique<Buffer>()),
      write_buffer_(std::make_unique<Buffer>()) {}

auto Connection::GetFd() const noexcept -> int { return socket_->GetFd(); }
auto Connection::GetSocket() noexcept -> Socket * { return socket_.get(); }

void Connection::SetEvents(uint32_t events) { events_ = events; }
auto Connection::GetEvents() const noexcept -> uint32_t { return events_; }
void Connection::SetRevents(uint32_t revents) { revents_ = revents; }
auto Connection::GetRevents() const noexcept -> uint32_t { return revents_; }

void Connection::SetCallback(std::function<void(Connection *)> callback) {
  // 值的方式捕获callback，捕获this是lambda像成员函数一样。
  // callback(this)
  // 为可调用对象。将成员callback_设置lambda,在lam中调用callback(参数为this)
  callback_ = [callback, this]() { return callback(this); };
}
auto Connection::GetCallback() noexcept -> std::function<void()> {
  return callback_;
}

/* for Buffer */
auto Connection::FindAndPopTill(const std::string &target)
    -> std::optional<std::string> {
  return read_buffer_->FindAndPopTill(target);
}

auto Connection::GetReadBufferSize() const noexcept -> size_t {
  return read_buffer_->Size();
}
auto Connection::GetWriteBufferSize() const noexcept -> size_t {
  return write_buffer_->Size();
}
void Connection::WriteToReadBuffer(const unsigned char *buf, size_t size) {
  read_buffer_->Append(buf, size);
}
void Connection::WriteToWriteBuffer(const unsigned char *buf, size_t size) {
  write_buffer_->Append(buf, size);
}
void Connection::WriteToReadBuffer(const std::string &str) {
  read_buffer_->Append(str);
}
void Connection::WriteToWriteBuffer(const std::string &str) {
  write_buffer_->Append(str);
}
void Connection::WriteToWriteBuffer(std::vector<unsigned char> &&other_buf) {
  write_buffer_->Append(std::move(other_buf));
}

auto Connection::Read() const noexcept -> const unsigned char * {
  return read_buffer_->Data();
}
auto Connection::ReadAsString() const noexcept -> std::string {
  auto string_view = read_buffer_->ToStringView();
  return {string_view.begin(), string_view.end()};
}

/* return std::pair<How many bytes read, whether the client exits> */
auto Connection::Recv() -> std::pair<ssize_t, bool> {
  int from_fd = GetFd();
  ssize_t read = 0, curr_read = 0;
  unsigned char buf[TEMP_BUF_SIZE];
  memset(buf, 0, sizeof(buf));
  while (true) {
    curr_read = recv(from_fd, buf, TEMP_BUF_SIZE, 0);
    if (curr_read > 0) {
      read += curr_read;
      WriteToReadBuffer(buf, curr_read);
      memset(buf, 0, sizeof(buf));
    } else if (curr_read == 0) {
      // read 返回0 客户端退出
      return {read, true};
    } else if (curr_read == -1 && errno == EINTR) {
      // 被打断，这不算错误
      continue;
    } else if (curr_read == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
      // 所有数据read
      break;
    } else {
      LOG_ERROR("HandleConnection: recv() error");
      return {read, true};
    }
  }
  return {read, false};
}
void Connection::Send() {
  ssize_t curr_write = 0;
  ssize_t write;
  const size_t to_write = GetWriteBufferSize();
  const unsigned char *buf = write_buffer_->Data();
  while (curr_write < to_write) {
    write = send(GetFd(), buf + curr_write, to_write - curr_write, 0);
    if (write < 0) {
      // 这三种不是错误
      if (errno != EINTR || errno != EAGAIN || errno != EWOULDBLOCK) {
        LOG_ERROR("Error in Connection::Send()");
        ClearWriteBuffer();
        return;
      }
      write = 0;
    }
    curr_write += write;
  }
  ClearWriteBuffer();
}
void Connection::ClearReadBuffer() noexcept { read_buffer_->Clear(); };
void Connection::ClearWriteBuffer() noexcept { write_buffer_->Clear(); }

void Connection::SetLooper(Looper *looper) noexcept { owner_looper_ = looper; }
auto Connection::GetLooper() noexcept -> Looper * { return owner_looper_; }
} // namespace Next