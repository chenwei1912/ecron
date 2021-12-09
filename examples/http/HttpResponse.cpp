#include "HttpResponse.h"
#include "Logger.h"

//#include <stdio.h>
//#include <cstring>
//#include <climits>

#include <unordered_set>


static const std::unordered_map<std::string, std::string> _SuffixType = {
    { ".html",  "text/html" },
    { ".xml",   "text/xml" },
    { ".xhtml", "application/xhtml+xml" },
    { ".txt",   "text/plain" },
    { ".rtf",   "application/rtf" },
    { ".pdf",   "application/pdf" },
    { ".word",  "application/nsword" },
    { ".png",   "image/png" },
    { ".gif",   "image/gif" },
    { ".jpg",   "image/jpeg" },
    { ".jpeg",  "image/jpeg" },
    { ".ico",   "image/x-icon"},
    { ".au",    "audio/basic" },
    { ".mpeg",  "video/mpeg" },
    { ".mpg",   "video/mpeg" },
    { ".avi",   "video/x-msvideo" },
    { ".gz",    "application/x-gzip" },
    { ".tar",   "application/x-tar" },
    { ".css",   "text/css "},
    { ".js",    "text/javascript "}
};

static const std::unordered_map<int, std::string> _CodeStatus = {
    { 200, "OK" },
    { 400, "Bad Request" },
    { 403, "Forbidden" },
    { 404, "Not Found" },
};

//static const std::unordered_map<int, std::string> _ErrorCodePath = {
//    { 400, "/400.html" },
//    { 403, "/403.html" },
//    { 404, "/404.html" },
//};

//static const std::unordered_set<std::string> _Route = {
//"/index",
//"/login",
//"/register",
//"/picture",
//"/video"
//};

static std::string GetFileType(std::string& path)
{
    std::string::size_type idx = path.find_last_of('.');
    if (idx == std::string::npos)
        return "text/plain";

    std::string suffix = path.substr(idx);
    auto it = _SuffixType.find(suffix);
    if (it != _SuffixType.end())
        return it->second;

    return "text/plain";
}


HttpResponse::HttpResponse()
        : http_code_(200)
        , keep_alive_(false)
{
}

HttpResponse::~HttpResponse()
{
    //UnmapFile();
}

void HttpResponse::init(int code, bool keepalive, const std::string& path, uint32_t len)
{
    http_code_ = code;
    keep_alive_ = keepalive;
    path_ = path;
    content_len_ = len;
    http_headers_.clear();
    //http_body_.clear();
}

//void HttpConn::make_response2(netlib::Buffer* buffer)
//{
//    buffer->write("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-type: text/html\r\n");
//    const char* temp = "<html><body><h1>12345</h1></body></html>";
//    size_t len = strlen(temp);
//    buffer->write("Content-length: " + std::to_string(len) + "\r\n\r\n");
//    buffer->write(temp, len);
//}

void HttpResponse::make_header(netlib::Buffer* buffer)
{
    auto it = _CodeStatus.find(http_code_); // _CodeStatus[http_code_]
    std::string str = fmt::format("HTTP/1.1 {} {}\r\n", http_code_, it->second);
    buffer->write(str);
    if (200 != http_code_) // all error, no other headers and body
    {
        buffer->write("Connection: close\r\n\r\n");
        return;
    }

    // other headers
    for (const auto& header : http_headers_)
    {
        str = fmt::format("{}: {}\r\n", header.first, header.second);
        buffer->write(str);
    }

    if (keep_alive_)
    {
        buffer->write("Connection: Keep-Alive\r\n");
    }
    else
    {
        buffer->write("Connection: close\r\n");
    }

    if (content_len_ > 0)
    {
        str = fmt::format("Content-type: {}\r\nContent-length: {}\r\n\r\n", 
                            GetFileType(path_), content_len_);
        buffer->write(str);
    }
}

