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

        connections_[conn] = http_conn;
    }
    else {
        connections_.erase(conn);
    }
}

void HttpServer::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
//    std::cout << buffer->begin_read() << " "<< len << " "
//        << buffer->readable_bytes() << " " << buffer->writable_bytes() << std::endl;

    netlib::LOGGER.write_log(netlib::LL_Info, "{}", buffer->begin_read());

    HttpConnPtr http_conn = connections_[conn];
    if (http_conn) {
        int ret = http_conn->parse(buffer->begin_read(), buffer->readable_bytes());
        if (-1 == ret) {
            netlib::LOGGER.write_log(netlib::LL_Error, "HttpServer parse error");
            conn->close();
            return;
        }

        buffer->has_readed(http_conn->count_parsed_);
        netlib::LOGGER.write_log(netlib::LL_Info, "HttpServer parsed bytes {}", ret);

        // messge timeout process

        if (http_conn->msgcomplete_) {
            netlib::LOGGER.write_log(netlib::LL_Info, 
                    "HttpServer parse OK! total bytes {}", http_conn->count_parsed_);

            netlib::BufferPtr buff_ptr = std::make_shared<netlib::Buffer>();
            
            http_conn->resp_make(buff_ptr.get());
//            const char* res = "HTTP/1.1 200 OK\r\n";
//            const char* res1 = "Content-Type: text/html\r\n";
//            const char* res2 = "Content-Length: 40\r\n";
//            const char* res3 = "Connection: Closed\r\n\r\n";
//            const char* res4 = "<html><body><h1>12345</h1></body></html>";
//            buff_ptr->write(res, strlen(res));
//            buff_ptr->write(res1, strlen(res1));
//            buff_ptr->write(res2, strlen(res2));
//            buff_ptr->write(res3, strlen(res3));
//            buff_ptr->write(res4, strlen(res4));
            
            conn->send(buff_ptr);

            http_conn->init();
        }
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



