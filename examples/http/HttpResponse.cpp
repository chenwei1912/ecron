#include "HttpResponse.h"
#include "Logger.h"

#include <stdio.h>
#include <cstring>
#include <climits>

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

static const std::unordered_map<int, std::string> _ErrorCodePath = {
    { 400, "/400.html" },
    { 403, "/403.html" },
    { 404, "/404.html" },
};

static const std::unordered_set<std::string> _Route = {
"/index",
"/login",
"/register",
"/picture",
"/video"
};

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

void HttpResponse::init()
{
    http_code_ = 200;
    keep_alive_ = false;
    http_headers_.clear();
    http_body_.clear();
}

void HttpResponse::set_code(int code)
{
    http_code_ = code;
}

void HttpResponse::set_keepalive(bool on)
{
    keep_alive_ = on;
}

//void HttpConn::make_response2(netlib::Buffer* buffer)
//{
//    buffer->write("HTTP/1.1 200 OK\r\nConnection: close\r\nContent-type: text/html\r\n");
//    const char* temp = "<html><body><h1>12345</h1></body></html>";
//    size_t len = strlen(temp);
//    buffer->write("Content-length: " + std::to_string(len) + "\r\n\r\n");
//    buffer->write(temp, len);
//}

void HttpResponse::make_response(netlib::Buffer* buffer, std::string& path)
{
    auto it = _CodeStatus.find(http_code_); // _CodeStatus[http_code_]
    std::string str = fmt::format("HTTP/1.1 {} {}\r\n", http_code_, it->second);
    buffer->write(str);

    if (keep_alive_)
    {
        buffer->write("Connection: Keep-Alive\r\n");
    }
    else
    {
        buffer->write("Connection: close\r\n");
    }

    str = fmt::format("Content-type: {}\r\n", GetFileType(path));
    buffer->write(str);

    FILE* pFile = fopen(path.data(), "rb");
    if (NULL == pFile)
    {
        //printf("open file error\n");
        return;
    }

    fseek(pFile, 0, SEEK_END);
    uint32_t FileSize = ftell(pFile);
    fseek(pFile, 0, SEEK_SET);

    str = fmt::format("Content-length: {}\r\n\r\n", FileSize);
    buffer->write(str);

    // other headers
    for (const auto& header : http_headers_)
    {
        str = fmt::format("{}: {}\r\n", header.first, header.second);
        buffer->write(str);
    }

    // if size is very large, multi send?
    // partition to send header and body
    uint32_t readbytes = 0;
    while (true) {
        buffer->ensure_writable(512);
        readbytes = fread(buffer->begin_write(), 1, 512, pFile);
        if (0 == readbytes)
            break;
        //printf("file read bytes: %d\n", readbytes);
        buffer->has_written(readbytes);
    }
    fclose(pFile);
}

