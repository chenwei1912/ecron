#ifndef _RPC_CLIENT_H_
#define _RPC_CLIENT_H_

#include "EventLoop.h"
#include "TcpClient.h"
#include "RpcChannel.h"


class RpcClient;
typedef std::function<void(RpcClient*)> RpcConnCallback;

class RpcClient // simple wrap
{
public:
    explicit RpcClient(ecron::net::EventLoop* loop);
    ~RpcClient();

    void set_conn_callback(RpcConnCallback f)
    {
        connection_f_ = f;
    }

    void connect(const char* strip, unsigned short port);
    void disconnect();

    inline bool is_connected() { return is_connected_; }
    inline RpcChannel* get_channel() { return &channel_; }

private:
    void on_connection(const ecron::net::TcpConnectionPtr& conn);
    //void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);

    ecron::net::TcpClient client_;
    RpcChannel channel_;

    RpcConnCallback connection_f_;
    bool is_connected_;

};

#endif // _RPC_CLIENT_H_