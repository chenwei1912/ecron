#ifndef ECRON_NET_HTTPREQUEST_H
#define ECRON_NET_HTTPREQUEST_H

#include "http_parser.h"

#include <unordered_map>


namespace ecron
{
namespace net
{

typedef std::unordered_map<std::string, std::string> stringmap_t;
//typedef header_t::iterator header_iter_t;

class HttpRequest
{
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

    void init();
    bool parse(const char* pdata, uint32_t len);
    bool parse_postbody();

    //inline uint32_t parsed_bytes() const { return count_parsed_; }

    //void set_method(const char*);
    //const std::string& get_method() const { return parser_.http_method;}

    std::string http_method_;
    //HttpUrl url;
    std::string http_url_;
    //std::string http_version_;
    bool keep_alive_;

    stringmap_t http_headers_;
    std::string http_header_field_; //field is waiting for value while parsing

    std::string http_body_;
    stringmap_t post_;

    //bool headercomplete_;
    bool msgcomplete_;
    uint32_t count_parsed_;

private:

    http_parser_settings settings_;
    http_parser parser_;

};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPREQUEST_H

