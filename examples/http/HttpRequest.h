#ifndef ECRON_NET_HTTPREQUEST_H
#define ECRON_NET_HTTPREQUEST_H


#include "Buffer.h"
#include <unordered_map>


namespace ecron
{
namespace net
{

class HttpRequest
{

friend class HttpTask;

public:
    //struct HttpUrl
    //{
    //    std::string schema;
    //    std::string host;
    //    unsigned short port;
    //    std::string path;
    //    std::string query;
    //    std::string fragment;
    //    std::string userinfo;
    //};

    HttpRequest();
    ~HttpRequest();

    //void set_method(const std::string& method) { method_ = method; }
    const std::string& get_method() const { return method_;}

    //void set_url(const std::string& url) { url_ = url; }
    const std::string& get_url() const { return url_;}

    //void set_keepalive(bool on) { keep_alive_ = on; }
    bool get_keepalive() const { return keep_alive_;}

    //void set_header(const std::string& key, const std::string& value);
    //void append_header(const std::string& key, const std::string& value);
    const std::string* get_header(const char* key) {
        auto it = headers_.find(key);
        if (it != headers_.end())
            return &(it->second);
        else
            return nullptr;
    }
    const std::string* get_header(const std::string& key) {
        auto it = headers_.find(key);
        if (it != headers_.end())
            return &(it->second);
        else
            return nullptr;
    }
    
    //size_t header_count() const { return headers_.size(); }

    Buffer& get_body() { return body_; }

private:
    void init();

    std::string method_;
    //HttpUrl url;
    std::string url_;
    //std::string version_;
    bool keep_alive_;

    std::unordered_map<std::string, std::string> headers_;

    Buffer body_;

};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPREQUEST_H

