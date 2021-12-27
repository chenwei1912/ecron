#include "HttpResponse.h"
#include "Logger.h"


using namespace ecron::net;


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


HttpResponse::HttpResponse()
{
}

HttpResponse::~HttpResponse()
{
    //UnmapFile();
}

void HttpResponse::init()
{
    code_ = 200;
    close_connection_ = true;
    content_type_.clear();
    content_len_ = 0;
    headers_.clear();
    body_.clear();
}

void HttpResponse::set_header(const std::string& key, const std::string& value)
{
    headers_[key] = value;
}

void HttpResponse::make_buffer(Buffer* buffer)
{
    auto it = _CodeStatus.find(code_); // _CodeStatus[http_code_]
    std::string str = fmt::format("HTTP/1.1 {} {}\r\n", code_, it->second);
    buffer->write(str);

    // add headers
    for (const auto& item : headers_)
    {
        str = fmt::format("{}: {}\r\n", item.first, item.second);
        buffer->write(str);
    }

    if (close_connection_)
        buffer->write("Connection: close\r\n");
    else
        buffer->write("Connection: Keep-Alive\r\n");

    if (!content_type_.empty())
    {
        str = fmt::format("Content-type: {}\r\n", content_type_); //GetFileType(path_)
        buffer->write(str);
    }

    if (content_len_ > 0)
    {
        str = fmt::format("Content-length: {}\r\n", content_len_);
        buffer->write(str);
    }

    buffer->write("\r\n");

    if (!body_.empty())
        buffer->write(body_);
}

