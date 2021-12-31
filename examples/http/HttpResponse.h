#ifndef ECRON_NET_HTTPRESPONSE_H
#define ECRON_NET_HTTPRESPONSE_H


#include "Buffer.h"
#include <unordered_map>


namespace ecron
{
namespace net
{

class HttpResponse
{

friend class HttpTask;

public:
    HttpResponse();
    ~HttpResponse();

    void set_code(int code) { code_ = code; }
    int get_code() const { return code_; }
    
    void set_closeconnection(bool on) { close_connection_ = on; }
    bool get_closeconnection() const { return close_connection_; }

    void set_content_type(const std::string& type) { content_type_ = type; }
    void set_content_length(uint32_t len) { content_len_ = len; }

    void set_header(const std::string& key, const std::string& value) {
        headers_[key] = value;
    }

    Buffer& get_body() { return body_; }

private:
    void init();
    void make_buffer(Buffer* buffer);

    int code_;
    bool close_connection_;
    std::string content_type_;
    uint32_t content_len_;
    std::unordered_map<std::string, std::string> headers_;
    Buffer body_;
};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPRESPONSE_H

