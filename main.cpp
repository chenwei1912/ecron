
#include <iostream>

#include "EventLoop.h"
#include "TcpServer.h"
#include "TcpClient.h"
#include "Logger.h"


netlib::EventLoop* _loop = nullptr;
netlib::TcpServer* _server = nullptr;
netlib::TimerId _timerid;

netlib::EventLoop* _loop2 = nullptr;

void my_task()
{
    std::cout << "my task" << std::endl;
}

void connection_timeout(netlib::TcpConnectionPtr conn)
{
    std::cout << "tcp connection data timeout" << std::endl;
    conn->close();
}

void func_connection(const netlib::TcpConnectionPtr& conn)
{
    if (conn->connected()) {
        std::cout << "tcp connection coming" << std::endl;
        //conn->close();

        _timerid = conn->get_loop()->add_timer(10, std::bind(connection_timeout, conn));
    }
    else
        std::cout << "tcp connection closed" << std::endl;
}
void func_message(const netlib::TcpConnectionPtr& conn, netlib::Buffer* buffer, size_t len)
{
    //buffer[len] = '\0';
    std::cout << buffer->begin_read() << " "<< len << " "
        << buffer->readable_bytes() << " " << buffer->writable_bytes() << std::endl;

    conn->send(buffer->begin_read(), buffer->readable_bytes());

    buffer->has_readed(len); // retrieve

    // reset timer
    conn->get_loop()->del_timer(_timerid);
    _timerid = conn->get_loop()->add_timer(10, std::bind(connection_timeout, conn));
}
void func_sendcomplete(const netlib::TcpConnectionPtr& conn)
{
    std::cout << "send data complete." << std::endl;
}

void idle_timeout()
{
    //std::cout << "idle do something: statistic or other" << std::endl;

    netlib::LOGGER.flush();
}




void thread_func()
{
    netlib::EventLoop loop;
    _loop = &loop;
    //loop.post(my_task);
    //loop.post(my_task);

    netlib::TcpServer server(&loop);
    _server = &server;
    server.set_connection_callback(func_connection);
    server.set_recv_callback(func_message);
    server.set_sendcomplete_callback(func_sendcomplete);
    server.set_io_threads(2); // std::thread::hardware_concurrency()
    server.start("192.168.17.19", 2007);

    loop.add_timer(6, idle_timeout, true);

    loop.loop();
    //std::cout << "server loop end" << std::endl;
}

void thread_func2()
{
    netlib::EventLoop loop;
    _loop2 = &loop;

    netlib::TcpClient client(&loop);
    client.set_connection_callback(func_connection);
    client.set_recv_callback(func_message);
    client.set_sendcomplete_callback(func_sendcomplete);
    client.connect("192.168.17.9", 4017);

    loop.loop();
    //std::cout << "client loop end" << std::endl;
}

int main(int argc, char* argv[])
{
    bool ret = netlib::LOGGER.init("server_log.log");
    if (!ret) {
        std::cout << "log init failed." << std::endl;
        return -1;
    }
    netlib::LOGGER.set_level(netlib::LL_Trace);

    std::thread t(thread_func);
    //std::thread t2(thread_func2);

    getchar();

//    if (_server) {
//        _server->stop();
//        std::cout << "server stoped." << std::endl;
//        
//    }
    if (_loop) {
        _loop->stop();
    }
//    if (_loop2) {
//        _loop2->stop();
//    }
    
    t.join();
//    if (t.get_id() == std::thread::id())
//        std::cout << "thread server is exited" << std::endl;
    //t2.join();
    netlib::LOGGER.release();
    std::cout << "main exit." << std::endl;
    return 0;
}



