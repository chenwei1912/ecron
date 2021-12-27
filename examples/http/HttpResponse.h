#ifndef ECRON_NET_HTTPRESPONSE_H
#define ECRON_NET_HTTPRESPONSE_H

#include "Buffer.h"

#include <unordered_map>


namespace ecron
{
namespace net
{

//enum HttpStatusCode
//{
//    kUnknown,
//    k200Ok = 200,
//    k301MovedPermanently = 301,
//    k400BadRequest = 400,
//    k404NotFound = 404,
//};

class HttpResponse
{
public:
    HttpResponse();
    ~HttpResponse();

    void init();
    //void init(int code, bool keepalive, const std::string& path, uint32_t len);

    void set_code(int code) { code_ = code; }
    int get_code() const { return code_; }
    
    void set_closeconnection(bool on) { close_connection_ = on; }
    bool get_closeconnection() const { return close_connection_; }

    void set_content_type(const std::string& type) { content_type_ = type; }
    void set_content_length(uint32_t len) { content_len_ = len; }

    void set_header(const std::string& key, const std::string& value);

    void set_body(const std::string& body) { body_ = body; }
    
    void make_buffer(Buffer* buffer);

    int code_;
    bool close_connection_;
    std::string content_type_;
    uint32_t content_len_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPRESPONSE_H

