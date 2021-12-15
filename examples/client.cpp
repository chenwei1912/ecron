
#include <iostream>

#include "EventLoop.h"
#include "TcpClient.h"
#include "Logger.h"



class MyClient
{
public:
    explicit MyClient(netlib::EventLoop* loop)
        : client_(loop, "MyClient")
    {
        client_.set_connection_callback(std::bind(&MyClient::on_connection, 
                this, std::placeholders::_1));
        client_.set_recv_callback(std::bind(&MyClient::on_recv, this, 
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        client_.set_sendcomplete_callback(std::bind(&MyClient::on_sendcomplete, 
                this, std::placeholders::_1));
    }
    ~MyClient() {}

    void connect(const char* strip, unsigned short port)
    {
        client_.connect(strip, port);
    }

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    void on_sendcomplete(const netlib::TcpConnectionPtr& conn);

    netlib::TcpClient client_;
};

void MyClient::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        //std::cout << "tcp connection coming" << std::endl;
        //_timerid = conn->get_loop()->add_timer(10, std::bind(connection_timeout, conn));
    }
    else {
        //std::cout << "tcp connection closed" << std::endl;
    }
}
void MyClient::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
//    std::cout << buffer->begin_read() << " "<< len << " "
//        << buffer->readable_bytes() << " " << buffer->writable_bytes() << std::endl;

    conn->send(buffer->begin_read(), buffer->readable_bytes());

    buffer->has_readed(len); // retrieve
}
void MyClient::on_sendcomplete(const netlib::TcpConnectionPtr& conn)
{
    //std::cout << "send data complete." << std::endl;
}

void idle_timeout()
{
    //std::cout << "idle do something." << std::endl;
    netlib::LOGGER.flush();
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " host_ip port" << std::endl;
        return -1;
    }

    bool ret = netlib::LOGGER.init("echoserver.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    MyClient client(&loop);

    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    client.connect(argv[1], port);

    loop.add_timer(3, idle_timeout, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}



