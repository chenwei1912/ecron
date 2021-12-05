#include "EventLoop.h"
#include "Logger.h"
#include "TimingWheel.h"


using namespace netlib;



EventLoop::EventLoop()
    : io_context_(1) // single thread
    , guard_(io_context_.get_executor()) //boost::asio::make_work_guard(io_context_)
    , stop_(false)
    //, thread_(std::this_thread::get_id())
    , tw_(new TimingWheel(this))
{
}

EventLoop::~EventLoop()
{
    LOG_TRACE("EventLoop {} destructing", static_cast<void*>(this));
}

void EventLoop::loop()
{
    thread_ = std::this_thread::get_id();

    // start timer service
    tw_->start();

    // log loop thread id
    // std::hash<std::thread::id>{}(thread_)

    LOG_TRACE("EventLoop {} start looping", static_cast<void*>(this)); // fmt::ptr(this)

    io_context_.run();

    LOG_TRACE("EventLoop {} stop looping", static_cast<void*>(this));
}

void EventLoop::stop()
{
    //stop_ = true;
    if (!io_context_.stopped())
        dispatch(std::bind(&EventLoop::stop_loop, this));
        //post(std::bind(&boost::asio::io_context::stop, &io_context_));
}

void EventLoop::post(std::function<void()> f)
{
    boost::asio::post(io_context_, f); // thread safe
}

void EventLoop::dispatch(std::function<void()> f)
{
    boost::asio::dispatch(io_context_, f);
//    if (in_loopthread())
//        f();
//    else
//        boost::asio::post(io_context_, f);
}

TimerId EventLoop::add_timer(size_t interval, TimerCallback task, bool repeat)
{
    return tw_->add_timer(interval, task, repeat);
}

void EventLoop::del_timer(TimerId tid)
{
    tw_->del_timer(tid);
}

//bool EventLoop::in_loopthread()
//{
//    return (thread_ == std::this_thread::get_id());
//}

void EventLoop::stop_loop()
{
    //guard_.reset();
    io_context_.stop();
}

