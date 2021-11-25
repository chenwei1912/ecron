// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef NETLIB_TCPCONNECTION_H
#define NETLIB_TCPCONNECTION_H

#include "Buffer.h"

#include <functional>
#include <memory>
#include <deque>

#include <boost/asio.hpp>



namespace netlib
{

class EventLoop;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::shared_ptr<Buffer> BufferPtr;

typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&, Buffer*, size_t)> RecvCallback;
typedef std::function<void(const TcpConnectionPtr&)> SendCompleteCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:

    explicit TcpConnection(EventLoop* loop); //boost::asio::ip::tcp::socket& sock
    ~TcpConnection();

    TcpConnection(const TcpConnection&) = delete;
    TcpConnection& operator=(const TcpConnection&) = delete;

    //void open();
    void assign(boost::asio::ip::tcp::socket& sock);
    void close(); // active shutdown connection

    void send(const char* data, size_t n);
    void handle_establish();

    inline boost::asio::ip::tcp::socket& get_socket()
    {
        return socket_;
    }
    inline EventLoop* get_loop()
    {
        return loop_;
    }
    inline bool connected()
    {
        return connected_;
    }

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
    inline void set_close_callback(const CloseCallback& cb) // internal use
    {
        close_callback_ = cb;
    }

private:
    void send_loop(BufferPtr b);
    void close_loop();
    //void disconnect();
    void async_recv();
    void async_send(); // const BufferPtr& ?

    void handle_recv(const boost::system::error_code& ec, size_t trans);
    void handle_send(const boost::system::error_code& ec, size_t trans);
    void handle_close();

    EventLoop* loop_;
    boost::asio::ip::tcp::socket socket_;

    bool connected_; // atomic

    Buffer recv_buffer_;
	//Buffer send_buffer_;
	std::deque<BufferPtr> send_buffer_;
	//std::mutex send_mutex_;

	ConnectionCallback connection_callback_;
	RecvCallback recv_callback_;
	SendCompleteCallback sendcomplete_callback_;
	CloseCallback close_callback_;
};



}// namespace netlib

#endif // NETLIB_TCPCONNECTION_H
