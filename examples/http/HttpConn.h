#ifndef _HTTP_CONN_H_
#define _HTTP_CONN_H_

//#include "http_parser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "TcpConnection.h"
#include "Buffer.h"

//#include <unordered_map>
#include <memory>

//#include <unistd.h>      // close
//#include <sys/stat.h>    // stat
//#include <sys/mman.h>    // mmap, munmap


//typedef std::unordered_map<std::string, std::string> header_t;

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

//struct HttpRequest
//{
//    std::string http_method;
//    std::string http_url;
//    int keep_alive;
//    //std::string http_version;
//    //HttpUrl url;

//    header_t    http_headers;
//    std::string http_header_field; //field is waiting for value while parsing

//    std::string http_body;
//};

//struct HttpResponse
//{
//    //std::string http_version;
//    int         http_code;
//    std::string http_phrase;

//    header_t    http_headers;

//    std::string http_body;

//    std::string GetResponse();
//    void        ResetResponse();
//};

class HttpConn
{
public:
    HttpConn();
    ~HttpConn();

    int init(const netlib::TcpConnectionPtr& conn);

    bool parse(netlib::Buffer* buffer);
    //int parse(const char* pdata, uint32_t len);
    void process();

    int http_code_;
    std::string http_path_;
    

private:
    void do_request(netlib::Buffer* buffer, HttpResponse* resp);
    void do_post();
    void parse_post();
    bool user_verify(const std::string& name, const std::string& pwd, int login);
    
    HttpRequest request_;
    std::weak_ptr<netlib::TcpConnection> conn_weak_;
};

typedef std::shared_ptr<HttpConn> HttpConnPtr;

#endif // _HTTP_CONN_H_

