#include <iostream>

#include "EventLoop.h"
#include "TcpClient.h"
#include "Logger.h"

#include "RpcChannel.h"
#include "RpcController.h"

#include "my_service.pb.h"


class RpcClient
{
public:
    explicit RpcClient(netlib::EventLoop* loop)
        : client_(loop, "RpcClient")
        , channel_(new RpcChannel())
        , stub_(channel_.get())
        , log_id_(0)
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

    void disconnect()
    {
        client_.disconnect();
    }

    void echo(const std::string& str);
    void add(int a, int b);

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);
    //void on_sendcomplete(const netlib::TcpConnectionPtr& conn);

    void on_done(RpcController* ctrl, google::protobuf::Message* resp);
    void test_method();

    netlib::TcpClient client_;

    RpcChannelPtr channel_;
    EchoService_Stub stub_;

    int log_id_;
};

void RpcClient::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        channel_->set_conn(conn);

        // test for async call
        client_.get_loop()->add_timer(1, std::bind(&RpcClient::test_method, this), true);
    }
    else {
        client_.get_loop()->stop();
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

void RpcClient::on_done(RpcController* ctrl, google::protobuf::Message* resp)
{
    EchoResponse* echo = dynamic_cast<EchoResponse*>(resp);
    if (nullptr != echo)
    {
        LOG_INFO("RpcClient recv {} echo: {}", ctrl->get_id(), echo->message());
        //add(3, 4);
        return;
    }
    AddResponse* add = dynamic_cast<AddResponse*>(resp);
    if (nullptr != add)
    {
        LOG_INFO("RpcClient recv {} add: {}", ctrl->get_id(), add->result());
        //echo("hello, rpc");
        return;
    }

    //delete ctl;
    //delete resp;
}

void RpcClient::test_method()
{
    echo("hello, rpc");
    add(3, 4);
}

void RpcClient::echo(const std::string& str)
{
    EchoRequest request;
    request.set_message(str);

    RpcController* ctrl = new RpcController; // delete in channel
    ctrl->set_id(log_id_++);
    EchoResponse* response = new EchoResponse; // delete in channel
    auto done = google::protobuf::NewCallback(this, &RpcClient::on_done, 
                        ctrl, static_cast<google::protobuf::Message*>(response));

    stub_.Echo(ctrl, &request, response, done);
}

void RpcClient::add(int a, int b)
{
    AddRequest request;
    request.set_a(a);
    request.set_b(b);

    RpcController* ctrl = new RpcController; // delete in channel
    ctrl->set_id(log_id_++);
    AddResponse* response = new AddResponse; // delete in channel
    auto done = google::protobuf::NewCallback(this, &RpcClient::on_done, 
                        ctrl, static_cast<google::protobuf::Message*>(response));

    stub_.Add(ctrl, &request, response, done);
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
    google::protobuf::ShutdownProtobufLibrary();
    std::cout << "main exit." << std::endl;
    return 0;
}
