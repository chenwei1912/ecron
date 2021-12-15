#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_


#include "EventLoop.h"
#include "TcpServer.h"

#include "ThreadPool.hpp"


class HttpServer
{
public:
    HttpServer(netlib::EventLoop* loop);
    ~HttpServer();

    bool start(const char* strip, unsigned short port);
    void on_idle();

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    void on_sendcomplete(const netlib::TcpConnectionPtr& conn);


    netlib::TcpServer server_;

    // task thread pool
    ThreadPool workers_;
};

#endif // _HTTP_SERVER_H_