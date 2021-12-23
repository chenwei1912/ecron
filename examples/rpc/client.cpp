#include "RpcClient.h"
#include "Logger.h"

#include "RpcChannel.h"
#include "RpcController.h"

#include "my_service.pb.h"
#include "gflags/gflags.h"

#include <iostream>


DEFINE_string(host, "192.168.17.19", "IP Address of server");
DEFINE_int32(port, 2007, "TCP Port of remote server");


class StubImpl
{
public:
    explicit StubImpl(RpcChannel* channel)
                : stub_(channel)
                , log_id_(0) {}
    ~StubImpl() {}

    void test_method()
    {
        echo("hello, rpc");
        add(3, 4);
    }
    
    void echo(const std::string& str)
    {
        EchoRequest request;
        request.set_message(str);

        EchoResponse* response = new EchoResponse;

        RpcController* ctrl = new RpcController;
        ctrl->set_id(log_id_++);
        auto done = google::protobuf::NewCallback(this, &StubImpl::on_done, 
                            ctrl, static_cast<google::protobuf::Message*>(response));
        stub_.Echo(ctrl, &request, response, done);
    }

    void add(int a, int b)
    {
        AddRequest request;
        request.set_a(a);
        request.set_b(b);

        AddResponse* response = new AddResponse;

        RpcController* ctrl = new RpcController;
        ctrl->set_id(log_id_++);
        auto done = google::protobuf::NewCallback(this, &StubImpl::on_done, 
                            ctrl, static_cast<google::protobuf::Message*>(response));
        stub_.Add(ctrl, &request, response, done);
    }

private:

    void on_done(RpcController* ctrl, google::protobuf::Message* resp)
    {
        EchoResponse* echo = dynamic_cast<EchoResponse*>(resp);
        if (nullptr != echo)
        {
            LOG_INFO("RpcClient echo {} result: {}", ctrl->get_id(), echo->message());
            return;
        }
        AddResponse* add = dynamic_cast<AddResponse*>(resp);
        if (nullptr != add)
        {
            LOG_INFO("RpcClient add {} result: {}", ctrl->get_id(), add->result());
            return;
        }

        //delete ctl;
        //delete resp;
    }

    EchoService_Stub stub_;

    int log_id_;
};


int main(int argc, char* argv[])
{
    // Parse gflags.
    gflags::ParseCommandLineFlags(&argc, &argv, false);
    
    bool ret = netlib::LOGGER.init("rpc_client.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    RpcClient client(&loop);
    StubImpl stub(client.get_channel());

    client.set_conn_callback([&loop, &stub](RpcClient* cl){
        if (cl->is_connected())
            loop.add_timer(1, std::bind(&StubImpl::test_method, &stub), true);
        else
            loop.quit();
    });
    client.connect(FLAGS_host.c_str(), FLAGS_port);

    loop.set_signal_handle([&loop](int signal){
        switch (signal)
        {
            case SIGINT:
            case SIGTERM:
                loop.quit();
                break;
            default:
                break;
        }
    });
    loop.add_signal(SIGINT);

    loop.add_timer(3, []{
        netlib::LOGGER.flush();
    }, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    google::protobuf::ShutdownProtobufLibrary();
    std::cout << "main exit." << std::endl;
    return 0;
}
