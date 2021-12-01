
#include <iostream>

#include "HttpServer.h"
#include "Logger.h"


void idle_timeout()
{
    netlib::LOGGER.flush();
}

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " host_ip port" << std::endl;
        return -1;
    }

    bool ret = netlib::LOGGER.init("httpserver.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    netlib::EventLoop loop;
    HttpServer server(&loop);
	
	uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
    server.start(argv[1], port);

    loop.add_timer(3, idle_timeout, true);

    loop.loop();

    // never run
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}


