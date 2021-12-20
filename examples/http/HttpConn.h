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


class HttpConn // represent a request-response process
{
public:
    HttpConn();
    ~HttpConn();

    void init(const netlib::TcpConnectionPtr& conn);

    bool parse(netlib::Buffer* buffer);
    //int parse(const char* pdata, uint32_t len);
    void send_complete(const netlib::TcpConnectionPtr& conn);

    void process();
    void process_body();

    inline bool is_complete() const { return request_.msgcomplete_; }
    inline bool is_body() const { return is_body_; }
    
private:
    void process_header(netlib::Buffer* buffer);
    void do_post();
    bool user_verify(const std::string& name, const std::string& pwd, int login);

    std::weak_ptr<netlib::TcpConnection> conn_weak_;
    
    HttpRequest request_;

    HttpResponse response_;
    bool is_body_;

    FILE* file_;
};

typedef std::shared_ptr<HttpConn> HttpConnPtr;

#endif // _HTTP_CONN_H_

