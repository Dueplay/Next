#include "../include/core/buffer.h"

namespace Next {

Buffer::Buffer(size_t initial_capacity) {
    buf_.reserve(initial_capacity);
}

void Buffer::Append(const unsigned char *new_char_data, size_t data_size) {
    buf_.insert(buf_.end(), new_char_data, new_char_data + data_size);
}

void Buffer::Append(const std::string &new_string_data) {
    Append(reinterpret_cast<const unsigned char *>(new_string_data.c_str()), new_string_data.size());
}

void Buffer::Append(std::vector<unsigned char> &&new_buf_data) {
    
    buf_.insert(buf_.end(), std::make_move_iterator(new_buf_data.begin()), std::make_move_iterator(new_buf_data.end()));
}

void Buffer::AppendToHead(const unsigned char *new_char_data, size_t data_size) {
    buf_.insert(buf_.begin(), new_char_data, new_char_data + data_size);
}

void Buffer::AppendToHead(const std::string &new_string_data) {
    AppendToHead(reinterpret_cast<const unsigned char *>(new_string_data.c_str()), new_string_data.size());
}

auto Buffer::FindAndPopTill(const std::string &target) -> std::optional<std::string> {
    std::optional<std::string> ret = std::nullopt;
    auto curr_content = ToStringView();
    auto pos = curr_content.find(target);
    if (pos != std::string::npos) {
        ret = curr_content.substr(0, pos + target.size());
        buf_.erase(buf_.begin(), buf_.begin() + pos + target.size());
    }
    return ret;
}

auto Buffer::Size() const noexcept -> size_t { return buf_.size(); }

auto Buffer::Capacity() const noexcept -> size_t { return buf_.capacity(); };

auto Buffer::Data() noexcept -> const unsigned char * { return buf_.data(); };

auto Buffer::ToStringView() const noexcept -> std::string_view {
    // string_view只是一个字符串的视图，构造函数可以避免拷贝，做到O(1)复杂度
    //std::string_view类的成员变量只包含两个：字符串指针和字符串长度。
    return {reinterpret_cast<const char *>(buf_.data()), buf_.size() };
}

void Buffer::Clear() noexcept { buf_.clear(); }
    
}