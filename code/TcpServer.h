// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef NETLIB_TCPSERVER_H
#define NETLIB_TCPSERVER_H


#include <functional>
#include <unordered_set>

#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "EventLoopPool.h"


namespace netlib
{

class EventLoop;

//typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;


class TcpServer
{
public:

    TcpServer(EventLoop* loop, const std::string& name);
    ~TcpServer();

    TcpServer(const TcpServer&) = delete;
    TcpServer& operator=(const TcpServer&) = delete;

    /// Set the number of threads for tcp connection's io.
    ///
    /// Must be called before @c start
    /// @param n
    /// - 0 means all I/O in accept loop's thread, no thread will created.
    ///     this is the default value.
    /// - 1 means all I/O in another thread.
    /// - N means a thread pool with N threads, new connections
    ///     are assigned on a round-robin basis.
    void set_io_threads(size_t n);

    // start listen and wait for accept new connection
    bool start(const char* strip, uint16_t port);

    inline EventLoop* get_loop() const { return loop_; }
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
    void stop();
    void accept_loop();
    void handle_accept(const TcpConnectionPtr& conn, const boost::system::error_code& ec);
    //void handle_accept(const boost::system::error_code& error, boost::asio::ip::tcp::socket peer);

    void remove_conn(const TcpConnectionPtr& conn);
    void remove_conn_loop(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    const std::string name_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::string ipport_;
    std::unordered_set<TcpConnectionPtr> connections_;
    EventLoopPool pool_;
    uint32_t count_;

    ConnectionCallback connection_callback_;
    RecvCallback recv_callback_;
    SendCompleteCallback sendcomplete_callback_;
};

}// namespace netlib

#endif // NETLIB_TCPSERVER_H