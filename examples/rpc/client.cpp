#include <iostream>

#include "EventLoop.h"
#include "TcpClient.h"
#include "Logger.h"

#include "RpcChannelImpl.h"
#include "my_service.pb.h"

class RpcClient
{
public:
    explicit RpcClient(netlib::EventLoop* loop)
        : client_(loop, "RpcClient")
        , channel_(new RpcChannelImpl())
        , stub_(channel_.get())
    {
        client_.set_connection_callback(std::bind(&RpcClient::on_connection, 
                this, std::placeholders::_1));
        client_.set_recv_callback(std::bind(&RpcClient::on_recv, this, 
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//        client_.set_sendcomplete_callback(std::bind(&RpcClient::on_sendcomplete, 
//                this, std::placeholders::_1));
    }
    ~RpcClient() {}

    void connect(const char* strip, unsigned short port)
    {
        client_.connect(strip, port);
    }

    void echo_method();

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    //void on_sendcomplete(const netlib::TcpConnectionPtr& conn);

    void on_done(RpcController* ctl, EchoResponse* resp);

    netlib::TcpClient client_;

    RpcChannelImplPtr channel_;
    EchoService_Stub stub_;
};

void RpcClient::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        channel_->set_conn(conn);

        client_.get_loop()->add_timer(1, std::bind(&RpcClient::echo_method, this), true);
    }
    else {
        //std::cout << "tcp connection closed" << std::endl;
    }
}

void RpcClient::on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
    channel_->process(buffer, len);
}

//void RpcClient::on_sendcomplete(const netlib::TcpConnectionPtr& conn)
//{

//}

void RpcClient::on_done(RpcController* ctl, EchoResponse* resp)
{
    LOG_INFO("RpcClient recv echo: {}", resp->message());
    //client_.disconnect();

    //delete ctl;
    //delete resp;
}

void RpcClient::echo_method()
{
    EchoRequest request;
    request.set_message("hello, rpc");

    RpcController* ctrl = new RpcController; // delete in channel
    
    EchoResponse* response = new EchoResponse; // delete in channel
    auto done = google::protobuf::NewCallback(this, &RpcClient::on_done, ctrl, response);

    stub_.Echo(ctrl, &request, response, done);
}

void idle_func()
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

    bool ret = netlib::LOGGER.init("rpc_client.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    RpcClient client(&loop);

    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    client.connect(argv[1], port);

    loop.add_timer(3, idle_func, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}