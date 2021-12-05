#include "HttpServer.h"
#include "HttpConn.h"
#include "Logger.h"
#include "SqlConnPool.h"


//#include <iostream>


HttpServer::HttpServer(netlib::EventLoop* loop)
    : loop_(loop)
    , server_(loop, "HttpServer")
{
    server_.set_connection_callback(std::bind(&HttpServer::on_connection, 
            this, std::placeholders::_1));
    server_.set_recv_callback(std::bind(&HttpServer::on_recv, this, 
            std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    server_.set_sendcomplete_callback(std::bind(&HttpServer::on_sendcomplete, 
            this, std::placeholders::_1));
}

HttpServer::~HttpServer()
{
    workers_.stop();
}

bool HttpServer::start(const char* strip, unsigned short port)
{
    bool ret = true;

    // database
    SqlConnPool* pool = SqlConnPool::Instance();
    ret = pool->Init("localhost", 3306, "root", "378540", "webserver", 2);
    if (!ret) {
        LOG_ERROR("HttpServer start database failed!");
        return false;
    }

    // workers
    ret = workers_.start(2, 100000);
    if (!ret) {
        LOG_ERROR("HttpServer start worker threads failed!");
        return false;
    }

    // network
    server_.set_io_threads(2);
    ret = server_.start(strip, port);
    if (!ret) {
        LOG_ERROR("HttpServer start listen failed!");
        return false;
    }

    return ret;
}

void HttpServer::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        HttpConnPtr http_conn = std::make_shared<HttpConn>();
        http_conn->init(conn);
        conn->set_context(http_conn);

        // security: complete messge timeout
    }
    else {

    }
}

void HttpServer::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
    LOG_INFO("{}", buffer->begin_read());

    HttpConnPtr http_conn = boost::any_cast<HttpConnPtr>(conn->get_context());
    bool complete = http_conn->parse(buffer);
    if (complete)
    {
        workers_.append(std::bind(&HttpConn::process, http_conn));

        // start a new HttpConn
        HttpConnPtr http_new = std::make_shared<HttpConn>();
        http_new->init(conn);
        conn->set_context(http_new);
    }
}

void HttpServer::on_sendcomplete(const netlib::TcpConnectionPtr& conn)
{
    //std::cout << "send data complete." << std::endl;
}

void HttpServer::on_idle()
{
    netlib::LOGGER.flush();
}



