#include "TcpServer.h"
#include "EventLoop.h"
#include "Logger.h"


using namespace netlib;
using namespace boost::asio;


void default_recv_callback(const netlib::TcpConnectionPtr& conn, 
                            netlib::Buffer* buffer, size_t len)
{
    buffer->has_readed(len);
}


TcpServer::TcpServer(EventLoop* loop)
    : loop_(loop)
    , acceptor_(loop->get_context())
    , pool_(loop)
    , recv_callback_(default_recv_callback)
{
}

TcpServer::~TcpServer()
{
    LOGGER.write_log(LL_Debug, "TcpServer dtor");
    stop();
}

void TcpServer::set_io_threads(size_t n)
{
    pool_.set_threads(n);
}

void TcpServer::start(const char* strip, unsigned short port)
{
    ip::tcp::endpoint ep(ip::address_v4::from_string(strip), port);
    //ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), port);

    boost::system::error_code ec;
    acceptor_.open(ep.protocol(), ec);
    if (ec)
        return;

    // prefer SO_EXCLUSIVEADDRUSE on Windows
    boost::asio::socket_base::reuse_address reuseOption(true);
    acceptor_.set_option(reuseOption, ec);
    if (ec)
        return;

    boost::asio::socket_base::enable_connection_aborted abortOption(true);
    acceptor_.set_option(abortOption, ec);
    if (ec)
        return;

    acceptor_.bind(ep, ec);
    if (ec)
        return;

    // start listen
    acceptor_.listen(socket_base::max_connections, ec);
    if (ec)
        return;

    pool_.start();

    async_accept();
    
}

void TcpServer::stop()
{
    pool_.stop();

    if (acceptor_.is_open())
        acceptor_.close();

    // close all connections
    for (auto& item : connections_)
    {
        item->close();
    }
    connections_.clear();
}

void TcpServer::async_accept()
{
    // ensure connection's all callback function
    // is called in relate io_context
    EventLoop* ioloop = pool_.get_loop();
    auto conn = std::make_shared<TcpConnection>(ioloop);
    conn->set_connection_callback(connection_callback_);
    conn->set_recv_callback(recv_callback_);
    conn->set_sendcomplete_callback(sendcomplete_callback_);
    conn->set_close_callback(std::bind(&TcpServer::remove_conn, this, 
                                std::placeholders::_1));

    acceptor_.async_accept(conn->get_socket(), std::bind(&TcpServer::handle_accept, this, 
                            conn, std::placeholders::_1));
//    acceptor_.async_accept(std::bind(&TcpServer::handle_accept, 
//                            this, std::placeholders::_1, std::placeholders::_2));
}

void TcpServer::handle_accept(const TcpConnectionPtr& conn, const boost::system::error_code& ec)
{
    do {
        if (ec)
        {
            //std::cout << "accept operation aborted" << std::endl;

            if (boost::asio::error::operation_aborted == ec.value())
                return; // tcp server close and async op is canceled
            else
                //WRITELOG(LL_ERROR, "server accept error: %d", error.value());
                break;
        }

        connections_.insert(conn);
        EventLoop* ioloop = conn->get_loop();
        ioloop->post(std::bind(&TcpConnection::handle_establish, conn));
    } while (false);

    async_accept();
}

//void TcpServer::handle_accept(const boost::system::error_code& ec, 
//                                boost::asio::ip::tcp::socket peer)
//{
//    do {
//        if (ec)
//        {
//            std::cout << "accept operation aborted" << std::endl;

//            if (boost::asio::error::operation_aborted == ec.value())
//                return; // tcp server close
//            else
//                //WRITELOG(LL_ERROR, "server accept error: %d", error.value());
//                break;
//        }

//        std::cout << "accept connection." << std::endl;
//        auto conn = std::make_shared<TcpConnection>(_test_loop);
//        conn->assign(peer);
//        conn->set_connection_callback(connection_callback_);
//        conn->set_recv_callback(recv_callback_);
//        conn->set_sendcomplete_callback(sendcomplete_callback_);
//        conn->set_close_callback(std::bind(&TcpServer::remove_conn, this, 
//                                    std::placeholders::_1));

//        connections_.insert(conn);

//        std::cout << "handle_accept, thread id " << std::this_thread::get_id() << std::endl;
//        EventLoop* ioloop = conn->get_loop();
//        if (ioloop != loop_)
//            std::cout << "loop accept diff " << ioloop << "-" << loop_ << std::endl;
//        else
//            std::cout << "loop accept same" << std::endl;
//        if (conn->get_socket().get_executor() == acceptor_.get_executor())
//            std::cout << "io context same " << std::endl;
//        else
//            std::cout << "io context diff " << std::endl;

//        ioloop->post(std::bind(&TcpConnection::handle_establish, conn));
//    } while (false);

//    async_accept();
//}

// this function is called in conn's loop for conn's close_callback
void TcpServer::remove_conn(const TcpConnectionPtr& conn)
{
    loop_->post(std::bind(&TcpServer::remove_conn_loop, this, conn));
}

// this function is called in server's loop
void TcpServer::remove_conn_loop(const TcpConnectionPtr& conn)
{
    //LOGGER.write_log(LL_Info, "TcpServer disconnect a connection");
    size_t n = connections_.erase(conn);
    //assert(n == 1);
}

