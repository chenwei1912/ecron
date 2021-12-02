#include "HttpServer.h"
#include "Logger.h"

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
    connections_.clear();
}

void HttpServer::start(const char* strip, unsigned short port)
{
    server_.set_io_threads(2);
    server_.start(strip, port);
}

void HttpServer::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        HttpConnPtr http_conn = std::make_shared<HttpConn>();
        http_conn->init();
        conn->set_context(http_conn);
    }
    else {

    }
}

void HttpServer::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
    netlib::LOGGER.write_log(netlib::LL_Info, "{}", buffer->begin_read());

    HttpConnPtr http_conn = boost::any_cast<HttpConnPtr>(conn->get_context());
    bool complete = http_conn->process(buffer);

    // messge timeout process

    if (complete)
    {
        netlib::BufferPtr buff_ptr = std::make_shared<netlib::Buffer>();
        HttpResponse resp;
        resp.init();
        http_conn->do_request(buff_ptr.get(), &resp);

        // send response
        conn->send(buff_ptr);

        if (0 == resp.get_keepalive())
            conn->close();

        http_conn->init();
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



