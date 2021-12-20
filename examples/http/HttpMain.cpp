
#include <iostream>

#include "HttpServer.h"
#include "Logger.h"

#include <boost/asio.hpp>


void dns_test()
{
    std::string svr_host = "www.163.com";
    std::string svr_port = "80";
    
    boost::asio::io_context ioc;
    boost::asio::ip::tcp::resolver resolver(ioc);

    boost::system::error_code ec;
    boost::asio::ip::tcp::resolver::results_type results = resolver.resolve(svr_host, svr_port, ec);
    if (ec)
    {
        std::cout << "resovle dns error: " << ec.message() << std::endl;
        return;
    }
    
    for (auto it = results.cbegin(); it != results.cend(); ++it)
    {
        boost::asio::ip::tcp::endpoint ep = it->endpoint();
        std::cout << ep << std::endl;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " host_ip port" << std::endl;
        return -1;
    }

    bool ret = false;
    ret = netlib::LOGGER.init("httpserver.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    HttpServer server(&loop);

    dns_test();
	
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    ret = server.start(argv[1], port);
    if (!ret) {
        std::cout << "http server start failed." << std::endl;
        return -2;
    }

    loop.add_timer(3, std::bind(&HttpServer::on_idle, &server), true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}



