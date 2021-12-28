
#include "RpcServer.h"
#include "Logger.h"
#include "gflags/gflags.h"

#include "my_service.pb.h"

#include <iostream>


DEFINE_string(host, "192.168.17.19", "IP Address of server");
DEFINE_int32(port, 2007, "TCP Port of remote server");


class EchoServiceImpl : public EchoService
{
public:
    EchoServiceImpl() {}
    virtual ~EchoServiceImpl() {}

    virtual void Echo(google::protobuf::RpcController* controller,
                       const ::EchoRequest* request,
                       EchoResponse* response,
                       google::protobuf::Closure* done)
    {
        LOG_TRACE("EchoService method Echo : {}", request->message());
        response->set_message(request->message());

        // fill RpcController result
        //if (nullptr != controller)
            //controller->SetFailed("xxx");
        if (nullptr != done)
            done->Run();
    }

    virtual void Add(google::protobuf::RpcController* controller,
                       const AddRequest* request,
                       AddResponse* response,
                       google::protobuf::Closure* done)
    {
        int32_t a = request->a();
	    int32_t b = request->b();

        LOG_TRACE("EchoService method Add : {},{}", a, b);

	    response->set_result(a + b);

        // fill RpcController result
        //if (nullptr != controller)
            //controller->SetFailed("xxx");

        if (nullptr != done)
            done->Run();
    }
};


int main(int argc, char* argv[])
{
    // Parse gflags.
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    bool ret = false;
    ret = ecron::LOGGER.init("rpc_server.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    ecron::LOGGER.set_level(ecron::LL_Trace);

    ecron::net::EventLoop loop;
    ecron::net::RpcServer server(&loop);

    EchoServiceImpl echo_service;
    server.register_service(&echo_service);
	
    ret = server.start(FLAGS_host.c_str(), FLAGS_port);
    if (!ret) {
        std::cout << "http server start failed." << std::endl;
        return -2;
    }

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
    //loop.remove_signal(SIGINT);

    loop.add_timer(3, []{
        //std::cout << "idle do something." << std::endl;
        ecron::LOGGER.flush();
    }, true);

    loop.loop();

    // never run
    ecron::LOGGER.release();
    google::protobuf::ShutdownProtobufLibrary();
    std::cout << "main exit." << std::endl;
    return 0;
}



