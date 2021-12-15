
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
            //controller->error_code_ = 0;
        
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
            //controller->error_code_ = 0;

        done->Run();
    }
};


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

    loop.add_timer(3, std::bind(&RpcServer::on_idle, &server), true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}



