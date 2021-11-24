// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef NETLIB_TCPCLIENT_H
#define NETLIB_TCPCLIENT_H


#include <functional>

#include <boost/asio.hpp>

#include "TcpConnection.h"


namespace netlib
{

class EventLoop;


class TcpClient
{
public:
    static const size_t Intervals;

    explicit TcpClient(EventLoop* loop);
    ~TcpClient();

    TcpClient(const TcpClient&) = delete;
    TcpClient& operator=(const TcpClient&) = delete;

    void connect(const char* strip, unsigned short port, size_t seconds = 5);
    void disconnect();

    inline void set_connection_callback(const ConnectionCallback& cb)
    {
        connection_callback_ = cb;
    }
    inline void set_recv_callback(const RecvCallback& cb)
    {
        recv_callback_ = cb;
    }
    inline void set_sendcomplete_callback(const SendCompleteCallback& cb)
    {
        sendcomplete_callback_ = cb;
    }

private:
    void async_connect();
    void async_timer(size_t seconds);

    
    void handle_connect(const boost::system::error_code& ec);
    void handle_timeout(const boost::system::error_code& ec);

    void remove_conn(const TcpConnectionPtr& conn);
    void remove_conn_loop(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    TcpConnectionPtr conn_;
    boost::asio::ip::tcp::endpoint ep_;
    boost::asio::steady_timer timer_; // improve batch process using timewheel
    size_t timeout_;

    ConnectionCallback connection_callback_;
    RecvCallback recv_callback_;
    SendCompleteCallback sendcomplete_callback_;
};

}// namespace netlib

#endif // NETLIB_TCPCLIENT_H