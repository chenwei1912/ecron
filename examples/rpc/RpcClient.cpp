#include "RpcClient.h"
#include "Logger.h"

using namespace ecron::net;


RpcClient::RpcClient(EventLoop* loop)
    : client_(loop, "RpcClient")
    , is_connected_(false)
{
    client_.set_connection_callback(std::bind(&RpcClient::on_connection, this, 
                                    std::placeholders::_1));
    client_.set_recv_callback(std::bind(&RpcClient::on_recv, this, std::placeholders::_1, 
                                    std::placeholders::_2, std::placeholders::_3));
//        client_.set_sendcomplete_callback(std::bind(&RpcClient::on_sendcomplete, 
//                this, std::placeholders::_1));
}

RpcClient::~RpcClient()
{
}

void RpcClient::connect(const char* strip, unsigned short port)
{
    client_.connect(strip, port);
}

void RpcClient::disconnect()
{
    client_.disconnect();
}

void RpcClient::on_connection(const TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        channel_.set_conn(conn);
        conn->set_context(&channel_);
        is_connected_ = true;
    }
    else {
        is_connected_ = false;
    }

    if (connection_f_)
        connection_f_(this);
}

void RpcClient::on_recv(const TcpConnectionPtr& conn, Buffer* buffer, size_t len)
{
    RpcChannel* channel = boost::any_cast<RpcChannel*>(conn->get_context());
    channel->process(buffer, len);
}

