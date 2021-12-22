#include "RpcClient.h"
#include "Logger.h"


RpcClient::RpcClient(netlib::EventLoop* loop)
    : client_(loop, "RpcClient")
    , is_connected_(false)
{
    client_.set_connection_callback(std::bind(&RpcClient::on_connection, 
            this, std::placeholders::_1));
    client_.set_recv_callback(std::bind(&RpcChannel::on_recv, &channel_, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
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

void RpcClient::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        channel_.set_conn(conn);
        is_connected_ = true;
    }
    else {
        is_connected_ = false;
    }

    if (connection_f_)
        connection_f_(this);
}

