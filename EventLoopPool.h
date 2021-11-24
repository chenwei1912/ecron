// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is an internal header file, you should not include this.

#ifndef NETLIB_EVENTLOOPPOOL_H
#define NETLIB_EVENTLOOPPOOL_H


#include <thread>
#include <memory>
#include <vector>



namespace netlib
{

class EventLoop;

class EventLoopPool
{
public:

    explicit EventLoopPool(EventLoop* loop);
    ~EventLoopPool();

    EventLoopPool(const EventLoopPool&) = delete;
    EventLoopPool& operator=(const EventLoopPool&) = delete;

    void set_threads(size_t n);
    void start();
    void stop();

    EventLoop* get_loop();

private:
    void run();

    size_t num_;
    EventLoop* baseloop_; // server's loop
    size_t next_;

	std::vector<std::unique_ptr<EventLoop>> loops_; // not stack, use heap
	std::vector<std::thread> threads_; // io threads
};

}// namespace netlib

#endif // NETLIB_EVENTLOOPPOOL_H