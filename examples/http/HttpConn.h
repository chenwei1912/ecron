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


class HttpConn
{
public:
    HttpConn();
    ~HttpConn();

    int init(const netlib::TcpConnectionPtr& conn);

    bool parse(netlib::Buffer* buffer);
    //int parse(const char* pdata, uint32_t len);
    void process();
    void process_body();

    inline bool is_complete() const { return complete_; }
    inline bool is_keepalive() const { return keep_alive_; }
    
private:
    void process_header(netlib::Buffer* buffer, HttpResponse* resp);
    //void process_body(const netlib::TcpConnectionPtr& conn);
    void do_post();
    bool user_verify(const std::string& name, const std::string& pwd, int login);

    int http_code_;
    std::string http_path_;
    HttpRequest request_;
    std::weak_ptr<netlib::TcpConnection> conn_weak_;

    bool keep_alive_;
    FILE* file_;
    bool complete_;
};

typedef std::shared_ptr<HttpConn> HttpConnPtr;

#endif // _HTTP_CONN_H_

