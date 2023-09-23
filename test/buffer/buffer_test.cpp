#include "../../src/include/core/buffer.h"
#include <iostream>
#include <vector>
int main() {
    Next::Buffer buf(2048);
    const char *str = "hello ";
    const char msg[1024] = "this is test of buffer.cpp.";
    const unsigned char *uchar_ptr = reinterpret_cast<const unsigned char *>(str);

    buf.AppendToHead(msg);
    buf.Append(uchar_ptr,6);
    buf.Append(std::string("world"));
    std::cout << buf.Data() << std::endl;
    std::cout << "buf size :" << buf.Size() << std::endl;
    std::cout << "buf capacity :" << buf.Capacity() << std::endl;

    buf.Append(std::vector<unsigned char>{',','g','x','j'});
    std::cout << buf.Data() << std::endl;

    buf.Clear();
    std::cout << "buf size :" << buf.Size() << std::endl;
    std::cout << "buf capacity :" << buf.Capacity() << std::endl;

    const std::string msg1 = "Greeting from beginning!";
    const std::string msg2 = "Farewell from end~";
    buf.AppendToHead(msg1);
    buf.Append(msg2);
    const std::string together = msg1 + msg2;
    if (buf.ToStringView() == together) {
        std::cout << "ToStringView succ,val: " << buf.ToStringView() << std::endl;
    }
    buf.Clear();
    /*
    在HTTP请求中，\r\n 被用来表示头部字段的结束，以及头部字段和消息正文之间的分隔。具体来说：

    "GET / HTTP/1.1\r\n" 是HTTP请求的第一行，表示请求方式、请求的URI和HTTP协议版本。
    "Connection: Keep-Alive\r\n" 是一个HTTP头部字段，指示客户端希望保持与服务器的持久连接。
    "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n" 是另一个HTTP头部字段，表示客户端的用户代理信息，通常是浏览器或HTTP客户端的标识信息。
    "Accept-Language: en-us\r\n" 是HTTP头部字段，表示客户端首选的自然语言偏好。
    \r\n 表示每个头部字段的结束，并且头部字段之间用空行（即两个 \r\n 之间没有其他字符）分隔。这是HTTP协议的规范要求，用于清晰地分隔HTTP请求的不同部分。
    */
    const std::string msg3 =
        "GET / HTTP/1.1\r\n"
        "Connection: Keep-Alive\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
        "Accept-Language: en-us\r\n"
        "\r\n";
    const std::string next_msg = "Something belongs to next message";
    buf.Append(msg3);
    buf.Append(next_msg);
    auto opt_str = buf.FindAndPopTill("\r\n\r\n");
    if ( opt_str.has_value() && opt_str.value() == msg3) {
        std::cout << "FindAndPopTill succ,val: " << std::endl << opt_str.value();
    }
    return 0;
}