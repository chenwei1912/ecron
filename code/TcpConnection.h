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
#include <boost/any.hpp>



namespace netlib
{

class EventLoop;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;

typedef std::function<void(const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&, Buffer*, size_t)> RecvCallback;
typedef std::function<void(const TcpConnectionPtr&)> SendCompleteCallback;
typedef std::function<void(const TcpConnectionPtr&)> CloseCallback;

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:

    TcpConnection(EventLoop* loop, const std::string& name);
    ~TcpConnection();

    TcpConnection(const TcpConnection&) = delete;
    TcpConnection& operator=(const TcpConnection&) = delete;

    void init();
    //void assign(boost::asio::ip::tcp::socket& sock);
    void close(); // active shutdown connection

    void send(const char* data, size_t n);
    void send(const BufferPtr& buffer);
    void handle_establish();

    inline boost::asio::ip::tcp::socket& get_socket() { return socket_; } // internal use
    inline EventLoop* get_loop() const { return loop_; }
    inline const std::string& name() const { return name_; }
    inline bool connected() const { return connected_; }

    std::string local_ip() const;
    uint16_t local_port() const;
    std::string remote_ip() const;
    uint16_t remote_port() const;

    void set_context(const boost::any& context) { context_ = context; }
    const boost::any& get_context() const { return context_; }

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
    void async_recv();
    void async_send(); // const BufferPtr& ?

    void handle_recv(const boost::system::error_code& ec, size_t trans);
    void handle_send(const boost::system::error_code& ec, size_t trans);
    void handle_close();

    EventLoop* loop_;
    const std::string name_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::endpoint remote_ep_;
    boost::asio::ip::tcp::endpoint local_ep_;

    bool connected_; // fixme atomic

    Buffer recv_buffer_;
	//Buffer send_buffer_;
	std::deque<BufferPtr> send_buffer_;
	//std::mutex send_mutex_;

	ConnectionCallback connection_callback_;
	RecvCallback recv_callback_;
	SendCompleteCallback sendcomplete_callback_;
	CloseCallback close_callback_;

	boost::any context_;
};



}// namespace netlib

#endif // NETLIB_TCPCONNECTION_H
