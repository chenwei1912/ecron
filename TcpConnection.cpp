#include "TcpConnection.h"
#include "EventLoop.h"
#include "Logger.h"

using namespace netlib;

//namespace asio = boost::asio;
//using tcp = asio::ip::tcp;
//using error_code = boost::system::error_code;



TcpConnection::TcpConnection(EventLoop* loop)
    : loop_(loop)
    , socket_(loop->get_context())
    //, socket_(loop->get_context(), boost::asio::ip::tcp::v4(), sock.release())
    , connected_(false)
{
    //recv_buffer_.reset();
}

TcpConnection::~TcpConnection()
{
    LOGGER.write_log(LL_Debug, "TcpConnection dtor");
    //boost::system::error_code ignored_ec;
    if (socket_.is_open())
    {
        //std::cout << "open socket closed" << std::endl;
        socket_.close();
    }

    send_buffer_.clear();
}

//void TcpConnection::open()
//{
//    do {
//        boost::asio::ip::tcp::no_delay delayOption(true);
//        socket_.set_option(delayOption);

//        boost::asio::socket_base::linger lingerOption(true, 0);
//        socket_.set_option(lingerOption);

//        boost::asio::ip::tcp::endpoint peer = socket_.remote_endpoint();

//        connected_ = true;

//        //memset(recv_buffer_, 0, sizeof(recv_buffer_));
//        async_recv();
//        return;
//    } while (false);

//    //boost::system::error_code ignored_ec;
//    //Disconnect(ignored_ec);
//}

void TcpConnection::assign(boost::asio::ip::tcp::socket& sock)
{
    socket_.assign(boost::asio::ip::tcp::v4(), sock.release());
}

void TcpConnection::close()
{
    loop_->dispatch(std::bind(&TcpConnection::close_loop, shared_from_this()));
}

void TcpConnection::close_loop()
{
    if (connected_)
    {
        LOGGER.write_log(LL_Info, "local shutdown connection");

        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        //socket_.close(ignored_ec);
        connected_ = false;
    }
}

void TcpConnection::send(const char* data, size_t n)
{
    if (nullptr == data || 0 == n || !connected_)
        return;

    // copy data to buffer because data is probably invalid when leave this function
    BufferPtr buff_ptr = std::make_shared<Buffer>(n);
    buff_ptr->write(data, n);

    loop_->dispatch(std::bind(&TcpConnection::send_loop, shared_from_this(), buff_ptr));
}

void TcpConnection::send_loop(BufferPtr b)
{
    bool sending = !send_buffer_.empty();
    send_buffer_.push_back(b);
    if (!sending) // not sending, start sending!
        async_send();
}

void TcpConnection::async_recv()
{
    // start async receive
    socket_.async_receive(boost::asio::buffer(recv_buffer_.begin_write(), recv_buffer_.writable_bytes()), 
                std::bind(&TcpConnection::handle_recv, shared_from_this(), 
                std::placeholders::_1, std::placeholders::_2));
//    boost::asio::async_read(socket_, boost::asio::buffer(recv_buffer_.begin_write(), recv_buffer_.writable_bytes()), 
//                boost::asio::transfer_at_least(1), 
//                std::bind(&TcpConnection::handle_read, shared_from_this(), 
//                std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::async_send()
{
    // start async send
    boost::asio::async_write(socket_, boost::asio::buffer(send_buffer_.front()->begin_read(), 
                send_buffer_.front()->readable_bytes()), 
                std::bind(&TcpConnection::handle_send, shared_from_this(), 
                std::placeholders::_1, std::placeholders::_2));
}

void TcpConnection::handle_recv(const boost::system::error_code& ec, size_t trans)
{
    if (ec) {
        handle_close();
        return;
    }

    LOGGER.write_log(LL_Debug, "TcpConnection recv data, len:{}", trans);

    // handle data
    recv_buffer_.has_written(trans);

    if (recv_callback_)
        recv_callback_(shared_from_this(), &recv_buffer_, trans);

    async_recv();
}

void TcpConnection::handle_send(const boost::system::error_code& ec, size_t trans)
{
    if (ec) {
        handle_close();
        return;
    }

    LOGGER.write_log(LL_Debug, "TcpConnection send data, len:{}", trans);

    if (!send_buffer_.empty()) {
        //send_buffer_.front()->has_readed(trans);
        send_buffer_.pop_front();
    }

    if (sendcomplete_callback_)
        sendcomplete_callback_(shared_from_this());

    if (!send_buffer_.empty())
        async_send();
}

void TcpConnection::handle_establish()
{
    do {
        boost::asio::ip::tcp::no_delay delayOption(true);
        socket_.set_option(delayOption);

        boost::asio::socket_base::linger lingerOption(true, 0);
        socket_.set_option(lingerOption);

        boost::asio::ip::tcp::endpoint peer = socket_.remote_endpoint();

        LOGGER.write_log(LL_Info, "tcp connection is established");

        // log peer
        connected_ = true;

        async_recv();

        if (connection_callback_) // connection established
            connection_callback_(shared_from_this());

        return;
    } while (false);

    //boost::system::error_code ignored_ec;
    //Disconnect(ignored_ec);
}

void TcpConnection::handle_close()
{
    // 10009 boost::asio::error::bad_descriptor
    // 995	 boost::asio::error::operation_aborted
    // 10054 boost::asio::error::connection_reset
    // 2	 boost::asio::error::eof
    // 10061 boost::asio::error::connection_refused
    if (connected_)
    {
        LOGGER.write_log(LL_Info, "peer shutdown connection");

        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        //socket_.close(ignored_ec);
        connected_ = false;
    }

    LOGGER.write_log(LL_Info, "tcp connection is closed");

    //TcpConnectionPtr guard(shared_from_this());
    if (connection_callback_)
        connection_callback_(shared_from_this()); // guard

    if (close_callback_)
        close_callback_(shared_from_this()); // guard
}


