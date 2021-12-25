#include "EventLoop.h"
#include "Logger.h"
#include "TimingWheel.h"


using namespace ecron::net;



EventLoop::EventLoop()
    : io_context_(1) // single thread
    , guard_(io_context_.get_executor()) //boost::asio::make_work_guard(io_context_)
    , stop_(false)
    //, thread_(std::this_thread::get_id())
    , tw_(new TimingWheel(this))
    , signals_(io_context_)
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

void EventLoop::quit()
{
    //stop_ = true;
    dispatch(std::bind(&EventLoop::quit_loop, this));
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

void EventLoop::set_signal_handle(SignalCallback cb)
{
    dispatch(std::bind(&EventLoop::set_signal_handle_loop, this, cb));
}

void EventLoop::add_signal(int signal_num)
{
    dispatch(std::bind(&EventLoop::add_signal_loop, this, signal_num));
}

void EventLoop::remove_signal(int signal_num)
{
    dispatch(std::bind(&EventLoop::remove_signal_loop, this, signal_num));
}

//bool EventLoop::in_loopthread()
//{
//    return (thread_ == std::this_thread::get_id());
//}

void EventLoop::quit_loop()
{
    //guard_.reset();
    if (!io_context_.stopped())
        io_context_.stop();
}

void EventLoop::handle_signal(const boost::system::error_code& ec, int signal_number)
{
    if (ec)
    {
        //LOG_ERROR("EventLoop signal error: {}", ec.value());
        return;
    }

    signals_.async_wait(std::bind(&EventLoop::handle_signal, this, 
                        std::placeholders::_1, std::placeholders::_2));

    if (cb_signale_)
        cb_signale_(signal_number);
}

void EventLoop::set_signal_handle_loop(SignalCallback cb)
{
    signals_.cancel();
    cb_signale_ = cb;
    signals_.async_wait(std::bind(&EventLoop::handle_signal, this, 
                        std::placeholders::_1, std::placeholders::_2));
}

void EventLoop::add_signal_loop(int signal_num)
{
    signals_.add(signal_num);
}

void EventLoop::remove_signal_loop(int signal_num)
{
    signals_.remove(signal_num);
}
