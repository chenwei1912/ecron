#ifndef ECRON_NET_RPCCLIENT_H
#define ECRON_NET_RPCCLIENT_H

#include "EventLoop.h"
#include "TcpClient.h"
#include "RpcChannel.h"


namespace ecron
{
namespace net
{

class RpcClient;
typedef std::function<void(RpcClient*)> RpcConnCallback;

class RpcClient // simple wrap
{
public:
    explicit RpcClient(EventLoop* loop);
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
    void on_connection(const TcpConnectionPtr& conn);
    void on_recv(const TcpConnectionPtr& conn, Buffer* buffer, size_t len);

    TcpClient client_;
    RpcChannel channel_;

    RpcConnCallback connection_f_;
    bool is_connected_;

};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_RPCCLIENT_H