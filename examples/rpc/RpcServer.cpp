#include "RpcServer.h"
//#include "HttpConn.h"
#include "Logger.h"
//#include "SqlConnPool.h"

#include "RpcChannel.h"

#include "google/protobuf/descriptor.h"



RpcServer::RpcServer(ecron::net::EventLoop* loop)
    : server_(loop, "RpcServer")
{
    server_.set_connection_callback(std::bind(&RpcServer::on_connection, 
            this, std::placeholders::_1));
    server_.set_recv_callback(std::bind(&RpcServer::on_recv, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//    server_.set_sendcomplete_callback(std::bind(&RpcServer::on_sendcomplete, 
//            this, std::placeholders::_1));
}

RpcServer::~RpcServer()
{
    //workers_.stop();
}

bool RpcServer::start(const char* strip, unsigned short port)
{
    bool ret = true;

    // database

    // workers

    // network
    server_.set_io_threads(1);
    ret = server_.start(strip, port);
    if (!ret) {
        LOG_ERROR("RpcpServer start listen failed!");
        return false;
    }

    return ret;
}

void RpcServer::register_service(::google::protobuf::Service* service)
{
    const google::protobuf::ServiceDescriptor* desc = service->GetDescriptor();
    services_[desc->full_name()] = service;
}

void RpcServer::on_connection(const ecron::net::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        RpcChannelPtr channel = std::make_shared<RpcChannel>();
        channel->set_conn(conn);
        channel->set_services(&services_);
        conn->set_context(channel);

        // security: messge timeout
    }
    else {

    }
}

void RpcServer::on_recv(const ecron::net::TcpConnectionPtr& conn, ecron::Buffer* buffer, size_t len)
{
    RpcChannelPtr channel = boost::any_cast<RpcChannelPtr>(conn->get_context());
    channel->on_recv(conn, buffer, len);
}

//void RpcServer::on_sendcomplete(const netlib::TcpConnectionPtr& conn)
//{

//}

