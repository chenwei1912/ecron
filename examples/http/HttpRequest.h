#ifndef _HTTP_REQUEST_H_
#define _HTTP_REQUEST_H_

#include "http_parser.h"

#include <unordered_map>
//#include <memory>


typedef std::unordered_map<std::string, std::string> stringmap_t;
//typedef header_t::iterator header_iter_t;

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

class HttpRequest
{
public:
    HttpRequest();
    ~HttpRequest();

    void init();
    bool parse(const char* pdata, uint32_t len);

    //const std::string& method() const { return parser_.http_method;}
    bool parse_post(const char* at, size_t length);

    std::string http_method_;
    std::string http_url_;
    //std::string http_version_;
    bool keep_alive_;
    //HttpUrl url;

    stringmap_t http_headers_;
    std::string http_header_field_; //field is waiting for value while parsing

    std::string http_body_;

    bool headercomplete_;
    bool msgcomplete_;
    uint32_t count_parsed_;

    stringmap_t post_;

private:

    http_parser_settings settings_;
    http_parser parser_;

};

#endif // _HTTP_REQUEST_H_

