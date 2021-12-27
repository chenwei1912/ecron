#ifndef ECRON_NET_RPCSERVER_H
#define ECRON_NET_RPCSERVER_H


#include "EventLoop.h"
#include "TcpServer.h"
#include "ThreadPool.h"

#include "google/protobuf/service.h"

#include <unordered_map>


namespace ecron
{
namespace net
{

class RpcServer
{
public:
    RpcServer(EventLoop* loop);
    ~RpcServer();

    bool start(const char* strip, unsigned short port, size_t n_io = 0, size_t n_worker = 0);

    void register_service(google::protobuf::Service* service);

private:
    void on_connection(const TcpConnectionPtr& conn);
    void on_recv(const TcpConnectionPtr& conn, Buffer* buffer, size_t len);
    //void on_sendcomplete(const netlib::TcpConnectionPtr& conn);
    

    TcpServer server_;

    ThreadPool workers_;
    size_t num_workers_;

    std::unordered_map<std::string, google::protobuf::Service*> services_;
};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_RPCSERVER_H