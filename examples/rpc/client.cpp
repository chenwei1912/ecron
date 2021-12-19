#include <iostream>

#include "EventLoop.h"
#include "TcpClient.h"
#include "Logger.h"

#include "RpcChannel.h"
#include "RpcController.h"

#include "my_service.pb.h"


class RpcClient;
typedef std::function<void(RpcClient*)> RpcConnCallback;

class RpcClient // simple wrap
{
public:
    explicit RpcClient(netlib::EventLoop* loop)
        : client_(loop, "RpcClient")
        //, channel_(new RpcChannel())
        , stub_(&channel_)
        , log_id_(0)
    {
        client_.set_connection_callback(std::bind(&RpcClient::on_connection, 
                this, std::placeholders::_1));
        client_.set_recv_callback(std::bind(&RpcChannel::on_recv, &channel_, 
                std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
//        client_.set_sendcomplete_callback(std::bind(&RpcClient::on_sendcomplete, 
//                this, std::placeholders::_1));
    }
    ~RpcClient() {}

    void connect(const char* strip, unsigned short port)
    {
        client_.connect(strip, port);
    }

//    void set_conn_callback(RpcConnCallback f)
//    {
//        connection_f_ = f;
//    }

    void disconnect()
    {
        client_.disconnect();
    }

    inline RpcChannel* get_channel() { return &channel_; }

    void echo(const std::string& str);
    void add(int a, int b);

private:
    void on_connection(const netlib::TcpConnectionPtr& conn);
    void on_recv(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len);

    void on_done(RpcController* ctrl, google::protobuf::Message* resp);
    void test_method();

    netlib::TcpClient client_;
    RpcChannel channel_;
    EchoService_Stub stub_;

    //RpcConnCallback connection_f_;
    netlib::TimerId timer_;

    int log_id_;
};

void RpcClient::on_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        channel_.set_conn(conn);
        //conn->set_context(channel_);

        timer_ = client_.get_loop()->add_timer(1, std::bind(&RpcClient::test_method, this), true);
    }
    else {
        client_.get_loop()->del_timer(timer_);
        client_.get_loop()->quit();
        //std::cout << "tcp connection closed" << std::endl;
    }

//    if (connection_f_)
//        connection_f_(this);
}

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

    //EchoService_Stub stub(&channel_);
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

    //EchoService_Stub stub(&channel_);
    stub_.Add(ctrl, &request, response, done);
}

//void conn_func(RpcClient* client)
//{
//    client->get_loop()->add_timer(1, std::bind(&RpcClient::test_method, client), true);
//}

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

    uint16_t port = static_cast<uint16_t>(atoi(argv[2]));

    bool ret = netlib::LOGGER.init("rpc_client.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    RpcClient client(&loop);

    //client.set_conn_callback(conn_func);
    client.connect(argv[1], port);

    loop.add_timer(3, idle_func, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    google::protobuf::ShutdownProtobufLibrary();
    std::cout << "main exit." << std::endl;
    return 0;
}
