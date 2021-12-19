
#include <iostream>

#include "RpcServer.h"
#include "Logger.h"

#include "my_service.pb.h"


class EchoServiceImpl : public EchoService
{
public:
    EchoServiceImpl() {}
    virtual ~EchoServiceImpl() {}

    virtual void Echo(::google::protobuf::RpcController* controller,
                       const ::EchoRequest* request,
                       ::EchoResponse* response,
                       ::google::protobuf::Closure* done)
    {
        LOG_TRACE("EchoService method Echo : {}", request->message());
        response->set_message(request->message());

        // fill RpcController result
        //if (nullptr != controller)
            //controller->SetFailed("xxx");
        if (nullptr != done)
            done->Run();
    }

    virtual void Add(::google::protobuf::RpcController* controller,
                       const ::AddRequest* request,
                       ::AddResponse* response,
                       ::google::protobuf::Closure* done)
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

void on_idle()
{
    //std::cout << "idle do something." << std::endl;
    netlib::LOGGER.flush();
}

int main(int argc, char* argv[])
{
//    if (argc < 3) {
//        std::cout << "Usage: " << argv[0] << " host_ip port" << std::endl;
//        return -1;
//    }

    bool ret = false;
    ret = netlib::LOGGER.init("rpc_server.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    RpcServer server(&loop);

    EchoServiceImpl echo_service;
    server.register_service(&echo_service);
	
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    ret = server.start(argv[1], port);
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

    loop.add_timer(3, [&loop](){
        //std::cout << "idle do something." << std::endl;
        netlib::LOGGER.flush();
    }, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    google::protobuf::ShutdownProtobufLibrary();
    std::cout << "main exit." << std::endl;
    return 0;
}



