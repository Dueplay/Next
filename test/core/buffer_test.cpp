/**
 * This is the unit test file for core/Buffer class
 */

#include "core/buffer.h"

#include <cstring>

#include "catch2/catch_test_macros.hpp"

/* for convenience reason */
using Next::Buffer;
using Next::INITIAL_BUFFER_CAPACITY;

TEST_CASE("[core/buffer]") {
  Buffer buf(INITIAL_BUFFER_CAPACITY);
  REQUIRE(buf.Size() == 0);
  REQUIRE(buf.Capacity() == INITIAL_BUFFER_CAPACITY);

  SECTION("appending c-str into buffer from both side") {
    const char msg1[1024] = "Greeting from beginning!";
    const char msg2[2014] = "Farewell from end~";
    buf.AppendToHead(msg1);
    buf.Append(msg2);
    // snprintf(char *str, size_t size, const char *format, ...);
    // 用于将格式化的数据写入字符串。它是 sprintf 函数的一个安全版本，因为它允许你指定目标字符串的最大长度，从而避免溢
    snprintf((char *)msg1 + strlen(msg1), strlen(msg2) + 1, "%s", msg2);  // msg1 + msg2
    CHECK(std::strncmp((char *)buf.Data(), msg1, std::strlen(msg1)) == 0);
    buf.Clear();
    CHECK(buf.Size() == 0);
  }

  SECTION("appending std::string into buffer from both side") {
    const std::string msg1 = "Greeting from beginning!";
    const std::string msg2 = "Farewell from end~";
    buf.AppendToHead(msg1);
    buf.Append(msg2);
    const std::string together = msg1 + msg2;
    CHECK(buf.ToStringView() == together);
    buf.Clear();
    CHECK(buf.Size() == 0);
  }

  SECTION("find and pop based on the target first found") {
    // 在 C++ 中，当你将多个字符串字面量放在一起，由双引号包围，
    // 并且它们之间仅由空白字符（如空格、换行）分隔时，编译器会自动将它们连接成一个单一的字符串字面量。
    const std::string msg =
        "GET / HTTP/1.1\r\n"
        "Connection: Keep-Alive\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
        "Accept-Language: en-us\r\n"
        "\r\n";
    const std::string next_msg = "Something belongs to next message";
    buf.Append(msg);
    buf.Append(next_msg);
    auto op_str = buf.FindAndPopTill("\r\n\r\n");
    CHECK((op_str.has_value() && op_str.value() == msg));
    CHECK(buf.ToStringView() == next_msg);
  }
}
