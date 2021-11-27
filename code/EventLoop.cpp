#include "EventLoop.h"
#include "Logger.h"
#include "TimingWheel.h"


using namespace netlib;



EventLoop::EventLoop()
    : guard_(io_context_.get_executor()) //boost::asio::make_work_guard(io_context_)
    , stop_(false)
    //, thread_(std::this_thread::get_id())
    , tw_(new TimingWheel(this))
{
}

EventLoop::~EventLoop()
{
    LOGGER.write_log(LL_Trace, "EventLoop {} destructing", static_cast<void*>(this));
}

void EventLoop::post(std::function<void()> f)
{
    boost::asio::post(io_context_, f); // thread safe
}

void EventLoop::dispatch(std::function<void()> f)
{
    //boost::asio::dispatch(io_context_, f);

    if (in_loopthread())
        f();
    else
        boost::asio::post(io_context_, f);
}

void EventLoop::stop()
{
    //stop_ = true;
    //work_.reset();
    io_context_.stop();
}

TimerId EventLoop::add_timer(size_t interval, TimerCallback task, bool repeat)
{
    return tw_->add_timer(interval, task, repeat);
}

void EventLoop::del_timer(TimerId tid)
{
    tw_->del_timer(tid);
}

bool EventLoop::in_loopthread()
{
    return (thread_ == std::this_thread::get_id());
}

void EventLoop::loop()
{
    thread_ = std::this_thread::get_id();

    // start timer service
    tw_->start();

    // log loop thread id
    // std::hash<std::thread::id>{}(thread_)

    LOGGER.write_log(LL_Trace, "EventLoop {} start looping", static_cast<void*>(this)); // fmt::ptr(this)

    io_context_.run();

    LOGGER.write_log(LL_Trace, "EventLoop {} stop looping", static_cast<void*>(this));
}

