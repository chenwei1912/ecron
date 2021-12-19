// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef NETLIB_EVENTLOOP_H
#define NETLIB_EVENTLOOP_H

#include "TimerId.h"

#include <functional>
#include <thread>

#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>



namespace netlib
{

//using work_guard_type = boost::asio::executor_work_guard<boost::asio::io_context::executor_type>;
typedef std::function<void(int)> SignalCallback;

class TimingWheel;

class EventLoop
{
public:

    EventLoop();
    ~EventLoop();

    EventLoop(const EventLoop&) = delete;
    EventLoop& operator=(const EventLoop&) = delete;

    void loop();
    void quit();

    // task queue it and wait for io_context's execution.
    void post(std::function<void()> f);
    // dispatch will call it rightaway if the dispatch-caller 
    // was called from io_context itself(in the same loop thread), 
    // but queue it otherwise.
    void dispatch(std::function<void()> f);

    TimerId add_timer(size_t interval, TimerCallback task, bool repeat = false);
    void del_timer(TimerId tid);

    void set_signal_handle(SignalCallback cb);
    void add_signal(int signal_num);
    void remove_signal(int signal_num);

    inline boost::asio::io_context& get_context() { return io_context_; }

private:
    void quit_loop();
    void handle_signal(const boost::system::error_code& ec, int signal_number);
    void set_signal_handle_loop(SignalCallback cb);
    void add_signal_loop(int signal_num);
    void remove_signal_loop(int signal_num);

    boost::asio::io_context io_context_;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> guard_;

	bool stop_;
	std::thread::id thread_;

    std::unique_ptr<TimingWheel> tw_;

    boost::asio::signal_set signals_; // FIXME: wrapper
    SignalCallback cb_signale_;

};

}// namespace netlib

#endif // NETLIB_EVENTLOOP_H