#ifndef _HTTP_CONN_H_
#define _HTTP_CONN_H_

#include "http_parser.h"
#include "Buffer.h"

#include <map>
#include <memory>

#include <unistd.h>      // close
#include <sys/stat.h>    // stat
#include <sys/mman.h>    // mmap, munmap


typedef std::map<std::string, std::string> header_t;
//typedef header_t::iterator header_iter_t;

struct HttpUrl
{
    std::string schema;
    std::string host;
    unsigned short port;
    std::string path;
    std::string query;
    std::string fragment;
    std::string userinfo;
};

struct HttpRequest
{
    std::string http_method;
    std::string http_url;
    //std::string http_version;
    //HttpUrl url;

    header_t    http_headers;
    std::string http_header_field; //field is waiting for value while parsing

    std::string http_body;
};

struct HttpResponse
{
    //std::string http_version;
    int         http_code;
    std::string http_phrase;

    header_t    http_headers;

    std::string http_body;

    std::string GetResponse();
    void        ResetResponse();
};

class HttpConn;
typedef std::shared_ptr<HttpConn> HttpConnPtr;

class HttpConn
{
public:
    HttpConn();
    ~HttpConn();

    int init();
    int parse(const char* pdata, uint32_t len);

    bool do_request();

    //void resp_init();
    void resp_make(netlib::Buffer* buffer);
    std::string GetFileType();

    HttpRequest request_;
    HttpUrl url_;

    //HttpResponse response_;
    int http_code_;
    std::string http_path_;

    //char* mmFile_; 
    //struct stat mmFileStat_;

    uint32_t count_parsed_;
    bool headercomplete_;
    bool msgcomplete_;
    

private:
    static int on_message_begin(http_parser* parser);
    static int on_url(http_parser* parser, const char* at, size_t length);
    static int on_status(http_parser* parser, const char* at, size_t length);
    static int on_header_field(http_parser* parser, const char* at, size_t length);
    static int on_header_value(http_parser* parser, const char* at, size_t length);
    static int on_headers_complete(http_parser* parser);
    static int on_body(http_parser* parser, const char* at, size_t length);
    static int on_message_complete(http_parser* parser);
    static int on_chunk_header(http_parser* parser);
    static int on_chunk_complete(http_parser* parser);

    http_parser_settings settings_;
    http_parser parser_;

};

#endif // _HTTP_CONN_H_

