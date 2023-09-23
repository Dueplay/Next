#ifndef NEXT_BUFFER_H
#define NEXT_BUFFER_H

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "../core/utils.h"
namespace Next {

static constexpr size_t INITIAL_BUFFER_CAPACITY = 1024;

class Buffer {
public:
    explicit Buffer(size_t initial_capacity = INITIAL_BUFFER_CAPACITY);
    
    ~Buffer() = default;
    
    auto operator=(const Buffer &other) -> Buffer & = default;

    NON_MOVEABLE(Buffer);

    void Append(const unsigned char *new_char_data, size_t data_size);

    void Append(const std::string &new_string_data);

    void Append(std::vector<unsigned char> &&new_buf_data);

    void AppendToHead(const unsigned char *new_char_data, size_t data_size);

    void AppendToHead(const std::string &new_string_data);

    auto FindAndPopTill(const std::string &target) -> std::optional<std::string>;

    auto Size() const noexcept -> size_t;

    auto Capacity() const noexcept -> size_t;

    auto Data() noexcept -> const unsigned char *;

    auto ToStringView() const noexcept -> std::string_view;

    void Clear() noexcept;
    
private:

    std::vector<unsigned char> buf_;
};


}// namespace Next
#endif // !NEXT_BUFFER_H