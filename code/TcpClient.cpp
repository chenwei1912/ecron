#include "TcpClient.h"
#include "EventLoop.h"
#include "Logger.h"

using namespace ecron::net;
using namespace boost::asio;


TcpClient::TcpClient(EventLoop* loop, const std::string& name)
    : loop_(loop)
    , name_(name)
    , interval_(0)
    , connecting_(ATOMIC_FLAG_INIT)
    , count_(1)
    //, recv_callback_(default_recv_callback)
{
}

TcpClient::~TcpClient()
{
    LOG_TRACE("TcpClient[{}] destructing", name_);
}

bool TcpClient::connect(const char* strip, uint16_t port, size_t interval)
{
    if (0 == port || 0 == interval)
        return false;

    if (connecting_.test_and_set())
        return false;

    //ip::tcp::endpoint ep(ip::address_v4::from_string(strip), port);
    ep_.address(ip::make_address_v4(strip));
    ep_.port(port);
    interval_ = interval;
    LOG_INFO("TcpClient[{}] start connect to {}:{}", name_, strip, port);

    loop_->post(std::bind(&TcpClient::connect_loop, this));
    return true;
}

void TcpClient::disconnect()
{
    if (!connecting_.test_and_set()) {
        connecting_.clear();
        return;
    }
    loop_->dispatch(std::bind(&TcpClient::disconnect_loop, this));
}

void TcpClient::connect_loop()
{
    std::string name = fmt::format("{}-{}:{}#{}", 
            name_, ep_.address().to_v4().to_string(), ep_.port(), count_);

    conn_ = std::make_shared<TcpConnection>(loop_, name);
    conn_->set_connection_callback(connection_callback_);
    conn_->set_recv_callback(recv_callback_);
    conn_->set_sendcomplete_callback(sendcomplete_callback_);
    conn_->set_close_callback(std::bind(&TcpClient::remove_conn, this, 
                                std::placeholders::_1));

    conn_->get_socket().async_connect(ep_, std::bind(&TcpClient::handle_connect, 
                                        this, std::placeholders::_1));
}

void TcpClient::disconnect_loop()
{
    loop_->del_timer(timer_);
    if (conn_) // connecting state
        if (conn_->connected()) // established
            conn_->close();
        else // try async connect
            conn_->get_socket().close();
    else // waiting state
        connecting_.clear();
}

void TcpClient::handle_connect(const boost::system::error_code& ec)
{
      // success or not, the socket was automatically opened
//    if (conn_->get_socket().is_open())
//        LOGGER.write_log(LL_Debug, "TcpClient socket is opened");

    if (ec) {
        // 110	 boost::asio::error::timed_out
        LOG_ERROR("TcpClient[{}] connect error : {}", name_, ec.value());

        conn_.reset();
        if (boost::asio::error::operation_aborted == ec.value()) {
            connecting_.clear();
            return;
        }

        // wait interval to restart connect
        timer_ = loop_->add_timer(interval_, std::bind(&TcpClient::handle_timeout, this));
        return;
    }

    ++count_;
    conn_->init();
    LOG_INFO("TcpClient[{}] establish connection to {}:{}", 
                        name_, conn_->remote_ip(), conn_->remote_port());
    conn_->handle_establish();
}

void TcpClient::handle_timeout()
{
    LOG_DEBUG("TcpClient[{}] interval elapsed, restart connect...", name_);
    connect_loop();
}

void TcpClient::remove_conn(const TcpConnectionPtr& conn)
{
    //assert(conn_ == conn);
    conn_.reset();
    connecting_.clear();
}

