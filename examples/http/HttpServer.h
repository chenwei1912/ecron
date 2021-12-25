#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_


#include "EventLoop.h"
#include "TcpServer.h"

#include "ThreadPool.h"


class HttpServer
{
public:
    HttpServer(ecron::net::EventLoop* loop);
    ~HttpServer();

    bool start(const char* strip, unsigned short port);
    void on_idle();

private:
    void on_connection(const ecron::net::TcpConnectionPtr& conn);
    void on_recv(const ecron::net::TcpConnectionPtr& conn, ecron::Buffer* buffer, size_t len);
    void on_sendcomplete(const ecron::net::TcpConnectionPtr& conn);


    ecron::net::TcpServer server_;

    // task thread pool
    ecron::ThreadPool workers_;
};

#endif // _HTTP_SERVER_H_