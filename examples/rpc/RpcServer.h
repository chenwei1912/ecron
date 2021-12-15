#ifndef _RPC_SERVER_H_
#define _RPC_SERVER_H_


#include "EventLoop.h"
#include "TcpServer.h"

#include "google/protobuf/service.h"

#include <unordered_map>


class RpcServer
{
public:
    RpcServer(netlib::EventLoop* loop);
    ~RpcServer();

    bool start(const char* strip, unsigned short port);
    void on_idle();

    void register_service(::google::protobuf::Service* service);

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    //void on_sendcomplete(const netlib::TcpConnectionPtr& conn);
    

    netlib::EventLoop* loop_;
    netlib::TcpServer server_;

    // task thread pool
    //ThreadPool workers_;

    std::unordered_map<std::string, ::google::protobuf::Service*> services_;
};

#endif // _RPC_SERVER_H_