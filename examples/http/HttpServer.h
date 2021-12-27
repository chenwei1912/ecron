#ifndef ECRON_NET_HTTPSERVER_H
#define ECRON_NET_HTTPSERVER_H


#include "EventLoop.h"
#include "TcpServer.h"
#include "ThreadPool.h"
#include "HttpTask.h"


namespace ecron
{
namespace net
{

class HttpServer
{
public:
    HttpServer(EventLoop* loop);
    ~HttpServer();
    
    bool start(const char* strip, unsigned short port, size_t n_io = 0, size_t n_worker = 0);

    void set_http_callback(const HttpCallback& cb)
    {
        http_cb_ = cb;
    }
    void set_http_body_callback(const HttpBodyCallback& cb)
    {
        http_body_cb_ = cb;
    }

private:
    void on_connection(const TcpConnectionPtr& conn);
    void on_recv(const TcpConnectionPtr& conn, Buffer* buffer, size_t len);
    void on_sendcomplete(const TcpConnectionPtr& conn);

    TcpServer server_;

    ThreadPool workers_;
    size_t num_workers_;

    HttpCallback http_cb_;
    HttpBodyCallback http_body_cb_;
};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_HTTPSERVER_H