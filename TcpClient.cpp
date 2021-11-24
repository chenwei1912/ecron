#include "TcpClient.h"
#include "EventLoop.h"
#include "Logger.h"

using namespace netlib;
using namespace boost::asio;


const size_t TcpClient::Intervals = 10;


TcpClient::TcpClient(EventLoop* loop)
    : loop_(loop)
    //, conn_(std::make_shared<TcpConnection>(loop))
    , timer_(loop->get_context())
    , timeout_(0)
    //, recv_callback_(default_recv_callback)
{
}

TcpClient::~TcpClient()
{
    LOGGER.write_log(LL_Debug, "TcpClient dtor");
}

void TcpClient::connect(const char* strip, unsigned short port, size_t seconds)
{
    if (nullptr == strip || 0 == port || 0 == 0 == seconds)
        return;
    if (conn_) // thread safe?
        return;

    //ip::tcp::endpoint ep(ip::address_v4::from_string(strip), port);
    ep_.address(ip::address_v4::from_string(strip));
    ep_.port(port);
    timeout_ = seconds;

    loop_->dispatch(std::bind(&TcpClient::async_connect, this));
}

void TcpClient::disconnect()
{
    if (conn_) // thread safe?
        conn_->close();
}

void TcpClient::async_connect()
{
    async_timer(timeout_);

    conn_ = std::make_shared<TcpConnection>(loop_);
    conn_->set_connection_callback(connection_callback_);
    conn_->set_recv_callback(recv_callback_);
    conn_->set_sendcomplete_callback(sendcomplete_callback_);
    conn_->set_close_callback(std::bind(&TcpClient::remove_conn, this, 
                                std::placeholders::_1));

    conn_->get_socket().async_connect(ep_, std::bind(&TcpClient::handle_connect, 
                                        this, std::placeholders::_1));
}

void TcpClient::async_timer(size_t seconds)
{
    timer_.expires_after(std::chrono::seconds(seconds));
    //timer_.expires_from_now(interval_timer);
    timer_.async_wait(std::bind(&TcpClient::handle_timeout,
                        this, std::placeholders::_1));
}

void TcpClient::handle_connect(const boost::system::error_code& ec)
{
    timer_.cancel();

    if (ec) {
        LOGGER.write_log(LL_Error, "handle_connect error : {}", ec.value());
        
//        if (boost::asio::error::connection_refused == ec.value()) {
//            std::cout << "connect is refused" << ec.value() << std::endl;
//        }
//        if (boost::asio::error::operation_aborted == ec.value()) { // timeout?
//            //if (!conn_->get_socket().is_open())
//            std::cout << "connect is cancel timeout" << ec.value() << std::endl;
//        }

        if (conn_)
            conn_.reset();
        async_timer(Intervals); // wait intervals to restart connect
        return;
    }

    conn_->handle_establish();
}

void TcpClient::handle_timeout(const boost::system::error_code& ec)
{
    if (ec)
        return;

    // log connect timeout
    if (conn_) {
        LOGGER.write_log(LL_Info, "connect timeout, close socket");
        conn_->get_socket().close(); // cancel connect and close socket
    } else {
        LOGGER.write_log(LL_Info, "interval elapsed, restart connect...");
        async_connect(); // restart connect
    }
}

void TcpClient::remove_conn(const TcpConnectionPtr& conn)
{
    //assert(conn_ == conn);
    conn_.reset();
}

