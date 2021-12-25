#include "HttpServer.h"
#include "HttpConn.h"
#include "Logger.h"
#include "SqlConnPool.h"


#include <iostream>


HttpServer::HttpServer(ecron::net::EventLoop* loop)
    : server_(loop, "HttpServer")
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
    ret = pool->Init("localhost", 3306, "root", "378540", "webserver", 1);
    if (!ret) {
        LOG_ERROR("HttpServer start database failed!");
        return false;
    }

    // workers
    if (0 != workers_.start(1, 100000))
    {
        LOG_ERROR("HttpServer start worker threads failed!");
        return false;
    }

    // network
    server_.set_io_threads(1);
    ret = server_.start(strip, port);
    if (!ret) {
        LOG_ERROR("HttpServer start listen failed!");
        return false;
    }

    return ret;
}

void HttpServer::on_connection(const ecron::net::TcpConnectionPtr& conn)
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

void HttpServer::on_recv(const ecron::net::TcpConnectionPtr& conn, ecron::Buffer* buffer, size_t len)
{
    LOG_INFO("{}", buffer->begin_read());

    HttpConnPtr http_conn = boost::any_cast<HttpConnPtr>(conn->get_context());
    if (!http_conn->parse(buffer))
    {
        conn->send("HTTP/1.1 400 Bad Request\r\n\r\n", 28);
        //conn->close();
    }

    if (http_conn->is_complete())
    {
        // on_request(HttpRequest*, HttpResponse*);
        workers_.append(std::bind(&HttpConn::process, http_conn));

        // queue<HttpConnPtr> for continuous serveral requests?

        //HttpConnPtr http_new = std::make_shared<HttpConn>();
        //http_new->init(conn);
        //conn->set_context(http_new);
    }
}

void HttpServer::on_sendcomplete(const ecron::net::TcpConnectionPtr& conn)
{
    HttpConnPtr http_conn = boost::any_cast<HttpConnPtr>(conn->get_context());
    if (http_conn->is_body())
        workers_.append(std::bind(&HttpConn::process_body, http_conn));
    else
        http_conn->send_complete(conn);
}

void HttpServer::on_idle()
{
    ecron::LOGGER.flush();
}



