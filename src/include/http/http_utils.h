#ifndef NEXT_HTTP_UTILS_H
#define NEXT_HTTP_UTILS_H

#include <map>
#include <string>
#include <vector>
namespace Next::Http {

static constexpr int READ_WRITE_PERMISSION = 0600;

static constexpr char PARAMETER_SEPARATOR[] = {"&"};
static constexpr char UNDERSCORE[] = {"_"};
static constexpr char SPACE[] = {" "};
static constexpr char DOT[] = {"."};
static constexpr char CRLF[] = {"\r\n"};
static constexpr char COLON[] = {":"};
static constexpr char DEFAULT_ROUTE[] = {"index.html"};
static constexpr char CGI_BIN[] = {"cgi-bin"};
static constexpr char CGI_PREFIX[] = {"cgi_tem"};

/* Common Header and Value */
static constexpr char HEADER_SERVER[] = {"Server"};
static constexpr char SERVER_NEXT[] = {"Next/1.0"};
static constexpr char HEADER_CONTENT_LENGTH[] = {"Content-Length"};
static constexpr char HEADER_CONTENT_TYPE[] = {"Content-Type"};
static constexpr char CONTENT_LENGTH_ZERO[] = {"0"};
static constexpr char HEADER_CONNECTION[] = {"Connection"};
static constexpr char CONNECTION_CLOSE[] = {"Close"};
static constexpr char CONNECTION_KEEP_ALIVE[] = {"Keep-Alive"};
static constexpr char HTTP_VERSION[] = {"HTTP/1.1"};

/* MIME Types */
static constexpr char MIME_HTML[] = {"text/html"};
static constexpr char MIME_CSS[] = {"text/css"};
static constexpr char MIME_PNG[] = {"image/png"};
static constexpr char MIME_JPG[] = {"image/jpg"};
static constexpr char MIME_JPEG[] = {"image/jpeg"};
static constexpr char MIME_GIF[] = {"image/gif"};
static constexpr char MIME_OCTET[] = {"application/octet-stream"};

/* Response status */
static constexpr char RESPONSE_OK[] = {"200 OK"};
static constexpr char RESPONSE_BAD_REQUEST[] = {"400 Bad Request"};
static constexpr char RESPONSE_NOT_FOUND[] = {"404 Not Found"};
static constexpr char RESPONSE_SERVICE_UNAVAILABLE[] = {
    "503 Service Unavailable"};

/* HTTP Method enum, only support GET/HEAD method now */
enum class Method { GET, HEAD, UNSUPPORTED };

/* HTTP version enum, only support HTTP 1.1 now */
enum class Version { HTTP_1_1, UNSUPPORTED };

/* Content Extension enum */
enum class Extension { HTML, CSS, PNG, JPG, JPEG, GIF, OCTET, UNSUPPORTED };

static const std::map<Method, std::string> METHOD_TO_STRING = {
    {Method::GET, "GET"},
    {Method::HEAD, "HEAD"},
    {Method::UNSUPPORTED, "UNSUPPORTED"}};

static std::map<Version, std::string> VERSION_TO_STRING = {
    {Version::HTTP_1_1, "HTTP/1.1"}, {Version::UNSUPPORTED, "UNSUPPORTED"}};

static std::map<Extension, std::string> EXTENSION_TO_STRING = {
    {Extension::HTML, "HTML"},   {Extension::CSS, "CSS"},
    {Extension::PNG, "PNG"},     {Extension::JPG, "JPG"},
    {Extension::JPEG, "JPEG"},   {Extension::GIF, "GIF"},
    {Extension::OCTET, "OCTET"}, {Extension::UNSUPPORTED, "UNSUPPORTED"}};

Method ToMethod(const std::string &str);

Version ToVersion(const std::string &str);

Extension ToExtension(const std::string &str);

auto ExtensionToMime(const Extension &extension) noexcept -> std::string;

/**
 * 将一个字符串分割成许多子字符串，按指定分隔符分割
 */
auto Spilt(const std::string &str, const char *delim = SPACE) noexcept
    -> std::vector<std::string>;

/**
 * 使用指定的分隔符连接字符串集合中的多个字符串
 */
auto Join(std::vector<std::string> &tokens, const char *delim = SPACE) noexcept
    -> std::string;

/**
 * 删除字符串前导和后面的分隔符
 */
auto Trim(const std::string &str, const char *delim = SPACE) noexcept
    -> std::string;

/**
 * 转为大写将字符串
 */
auto ToUpper(std::string str) noexcept -> std::string;

/**
 * 将 Trim + ToUpper 应用于字符串并返回其格式化版本
 */
auto Format(const std::string &) noexcept -> std::string;

// 文件系统函数
/**
 * 检查指定目录是否存在
 */
auto IsDirectoryExists(const std::string &directory_path) noexcept -> bool;

/**
 * 检查是否包含cgi path,以判断是否是一个cgi请求
 */
auto IsCgiRequest(const std::string &resource_url) noexcept -> bool;

/**
 * check if the specified_path file exists
 */
auto IsFileExists(const std::string &file_path) noexcept -> bool;

/**
 * Delete a file if exists
 */
auto DeleteFile(const std::string &file_path) noexcept -> bool;

/**
 * Tell the size of a file in bytes.
 * Assume this file already exists and is verified
 */
auto CheckFileSize(const std::string &file_path) noexcept -> size_t;

/**
 * Load the file appending to be back of a vector of unsigned char
 * able to contain binary data
 */
void LoadFile(const std::string &file_path,
              std::vector<unsigned char> &buffer) noexcept;
} // namespace Next::Http

#endif