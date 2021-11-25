#include "EventLoopPool.h"
#include "EventLoop.h"
#include "Logger.h"

//#include <memory>
#include <iostream>
#include <thread>



using namespace netlib;



EventLoopPool::EventLoopPool(EventLoop* loop)
    : num_(0)
    , baseloop_(loop)
    , next_(0)
    //, thread_(std::this_thread::get_id())
{
}

EventLoopPool::~EventLoopPool()
{
    LOGGER.write_log(LL_Debug, "EventLoopPool dtor");
}

void EventLoopPool::set_threads(size_t n)
{
    num_ = n;
}

void EventLoopPool::start()
{
    next_ = 0;

    for (size_t i = 0; i < num_; ++i)
    {
        EventLoop* p = new EventLoop();
        loops_.emplace_back(p);
        threads_.emplace_back(std::bind(&EventLoop::loop, p));
        if (threads_.back().get_id() == std::thread::id()) {
            LOGGER.write_log(LL_Critical, "EventLoopPool thread is not activated");
            std::terminate();
        }
    }
}

void EventLoopPool::stop()
{
    for (auto& item : loops_)
        item->stop();
    for (auto& item : threads_)
        if (item.joinable())
            item.join();

    threads_.clear();
    loops_.clear();
}

EventLoop* EventLoopPool::get_loop()
{
    // wait for thread running?
    EventLoop* loop = baseloop_;

    if (!loops_.empty()) {
        loop = loops_[next_].get();

        ++next_;
        next_ %= loops_.size(); // round-robin
    }

    return loop;
}

