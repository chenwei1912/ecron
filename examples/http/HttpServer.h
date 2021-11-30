#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_


#include "EventLoop.h"
#include "TcpServer.h"

#include "HttpConn.h"

#include <unordered_map>


class HttpServer
{
public:
    HttpServer(netlib::EventLoop* loop);
    ~HttpServer();

    void start(const char* strip, unsigned short port);

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    void on_sendcomplete(const netlib::TcpConnectionPtr& conn);
    void on_idle();

    netlib::EventLoop* loop_;
    netlib::TcpServer server_;

    std::unordered_map<netlib::TcpConnectionPtr, HttpConnPtr> connections_;
};

#endif // _HTTP_SERVER_H_