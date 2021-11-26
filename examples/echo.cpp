
#include <iostream>

#include "EventLoop.h"
#include "TcpServer.h"
#include "Logger.h"



class EchoServer
{
public:
    EchoServer(netlib::EventLoop* loop)
        : loop_(loop)
        , server_(loop, "EchoServer")
    {
        server_.set_connection_callback(std::bind(&EchoServer::on_connection, 
                this, std::placeholders::_1));
        server_.set_recv_callback(std::bind(&EchoServer::on_recv, this, 
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        server_.set_sendcomplete_callback(std::bind(&EchoServer::on_sendcomplete, 
                this, std::placeholders::_1));
    }
    ~EchoServer() {}

    void start(const char* strip, unsigned short port)
    {
        server_.start(strip, port);
    }

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    void on_sendcomplete(const netlib::TcpConnectionPtr& conn);

    netlib::EventLoop* loop_;
    netlib::TcpServer server_;
};

void EchoServer::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        std::cout << "tcp connection coming" << std::endl;
        //_timerid = conn->get_loop()->add_timer(10, std::bind(connection_timeout, conn));
    }
    else
        std::cout << "tcp connection closed" << std::endl;
}
void EchoServer::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
//    std::cout << buffer->begin_read() << " "<< len << " "
//        << buffer->readable_bytes() << " " << buffer->writable_bytes() << std::endl;

    conn->send(buffer->begin_read(), buffer->readable_bytes());

    buffer->has_readed(len); // retrieve
}
void EchoServer::on_sendcomplete(const netlib::TcpConnectionPtr& conn)
{
    //std::cout << "send data complete." << std::endl;
}

void idle_timeout()
{
    std::cout << "idle do something" << std::endl;

    netlib::LOGGER.flush();
}

int main(int argc, char* argv[])
{
    bool ret = netlib::LOGGER.init("echoserver.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    EchoServer server(&loop);
    server.start("192.168.17.19", 2007);

    loop.add_timer(3, idle_timeout, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}



