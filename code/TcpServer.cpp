#include "TcpServer.h"
#include "EventLoop.h"
#include "Logger.h"


using namespace boost::asio;

using namespace ecron::net;
using namespace ecron;



void default_recv_callback(const TcpConnectionPtr& conn, 
                            Buffer* buffer, size_t len)
{
    buffer->has_readed(len);
}


TcpServer::TcpServer(EventLoop* loop, const std::string& name)
    : loop_(loop)
    , name_(name)
    , acceptor_(loop->get_context())
    , pool_(loop)
    , count_(0)
    , recv_callback_(default_recv_callback)
{
}

TcpServer::~TcpServer()
{
    LOG_TRACE("TcpServer [{}] destructing", name_);
    stop();
}

void TcpServer::set_io_threads(size_t n)
{
    pool_.set_threads(n);
}

bool TcpServer::start(const char* strip, uint16_t port)
{
    ip::tcp::endpoint ep(ip::make_address_v4(strip), port);
    //ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), port);

    boost::system::error_code ec;
    acceptor_.open(ep.protocol(), ec);
    if (ec) {
        LOG_ERROR("TcpServer[{}] socket open error : {}", name_, ec.value());
        return false;
    }

    // prefer SO_EXCLUSIVEADDRUSE on Windows
    boost::asio::socket_base::reuse_address reuseOption(true);
    acceptor_.set_option(reuseOption, ec);
    if (ec) {
        LOG_ERROR("TcpServer[{}] socket reuse option error : {}", name_, ec.value());
        return false;
    }

    boost::asio::socket_base::enable_connection_aborted abortOption(true);
    acceptor_.set_option(abortOption, ec);
    if (ec) {
        LOG_ERROR("TcpServer[{}] socket enable_connection_aborted option error : {}", name_, ec.value());
        return false;
    }

    acceptor_.bind(ep, ec);
    if (ec) {
        LOG_ERROR("TcpServer[{}] socket bing error : {}", name_, ec.value());
        return false;
    }

    // start listen
    acceptor_.listen(socket_base::max_connections, ec);
    if (ec) {
        LOG_ERROR("TcpServer[{}] sokect listen error : {}", name_, ec.value());
        return false;
    }

    // start io pool
    pool_.start();

    loop_->post(std::bind(&TcpServer::accept_loop, this));

    ipport_ = fmt::format("{}:{}", strip, port);
    LOG_INFO("TcpServer[{}] start listen on {}", name_, ipport_);
    return true;
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

void TcpServer::accept_loop()
{
    // ensure connection's all callback function
    // is called in relate io_context
    ++count_;
    std::string name = fmt::format("{}-{}#{}", name_, ipport_, count_);

    EventLoop* ioloop = pool_.get_nextloop();
    auto conn = std::make_shared<TcpConnection>(ioloop, name);
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
            --count_;

            // 995	 boost::asio::error::operation_aborted
            // 24    boost::asio::error::no_descriptors
            LOG_ERROR("TcpServer[{}] accept error: {}", name_, ec.value());
            if (boost::asio::error::operation_aborted == ec.value())
                return; // tcp server close and async op is canceled
            else
                break;
        }

        connections_.insert(conn);

        conn->init();

        LOG_INFO("TcpServer[{}] accept connection [{}] from {}:{:d}", 
                    name_, conn->name(), conn->remote_ip(), conn->remote_port());

        EventLoop* ioloop = conn->get_loop();
        ioloop->dispatch(std::bind(&TcpConnection::handle_establish, conn));
    } while (false);

    accept_loop();
}

//void TcpServer::handle_accept(const boost::system::error_code& ec, 
//                                boost::asio::ip::tcp::socket peer)
//{
//    EventLoop* ioloop = pool_.get_nextloop();
//    auto conn = std::make_shared<TcpConnection>(ioloop);
//    conn->assign(peer);
//}

// this function is called in conn's loop for conn's close_callback
void TcpServer::remove_conn(const TcpConnectionPtr& conn)
{
    loop_->dispatch(std::bind(&TcpServer::remove_conn_loop, this, conn));
}

// this function is called in server's loop
void TcpServer::remove_conn_loop(const TcpConnectionPtr& conn)
{
    //LOGGER.write_log(LL_Info, "TcpServer disconnect a connection");
    connections_.erase(conn);
    //assert(n == 1);
}

