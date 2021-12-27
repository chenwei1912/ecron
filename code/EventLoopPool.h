// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is an internal header file, you should not include this.

#ifndef ECRON_NET_EVENTLOOPPOOL_H
#define ECRON_NET_EVENTLOOPPOOL_H


#include <thread>
#include <memory>
#include <vector>



namespace ecron
{
namespace net
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

    EventLoop* get_nextloop();

private:
    void run();

    size_t num_;
    EventLoop* baseloop_; // server's loop
    size_t next_;

	std::vector<std::unique_ptr<EventLoop>> loops_; // not stack, use heap
	std::vector<std::thread> threads_; // io threads
};

}// namespace net
}// namespace ecron

#endif // ECRON_NET_EVENTLOOPPOOL_H