#include "TimingWheel.h"
#include "EventLoop.h"
#include "Logger.h"

//#include <chrono>
//#include <functional>
#include <iostream>

//#include <boost/timer.hpp>
//#include <boost/chrono.hpp>


using namespace netlib;
using namespace boost::asio;


std::atomic<uint64_t> Timer::num_created_;
//const int TimingWheel::Count_Ticks = 60;
const size_t TimingWheel::Interval_Tick; // is not use in h file
//int Timer::Timer_Count = 0;


uint64_t Timer::generate_timerid()
{
	int x = rand() % 0xffffffff;
	int cur_time = time(nullptr);
	uint64_t num = num_created_.fetch_add(1);
	uint64_t ret = x | cur_time | num;
	//++Timer_Count;
	return ret;
}


TimingWheel::TimingWheel(EventLoop* loop)
    : loop_(loop)
    , timer_(loop->get_context())
    , cur_slot_(0)
{
}

TimingWheel::~TimingWheel()
{
    for (auto& item : wheel_) {
        for (auto& timer : item) {
            delete timer;
        }
        item.clear();
    }
}

void TimingWheel::start()
{
    //boost::posix_time::seconds interval(1);
    //boost::chrono::seconds interval_timer(1);

    timer_.expires_after(std::chrono::seconds(Interval_Tick));
    //timer_.expires_from_now(interval_timer);
    timer_.async_wait(std::bind(&TimingWheel::handle_timeout,
                        this, std::placeholders::_1));
}

void TimingWheel::stop()
{
    timer_.cancel();
    //assert(n == 1);
}

TimerId TimingWheel::add_timer(size_t interval, TimerCallback task, bool repeat)
{
    //std::shared_ptr<TwTimer> sp_t(new TwTimer());
    //std::shared_ptr<TimerSession> sp(MEMORYPOOL.MallocTimer(),
    //                            std::bind(&MemoryPool::FreeTask, &MEMORYPOOL,
    //                            std::placeholders::_1));
    if (0 == interval || !task)
        return TimerId();

    Timer* timer = new Timer(interval, task, repeat); // std::move(task)
    loop_->dispatch(std::bind(&TimingWheel::add_timer_loop, this, timer));
    return TimerId(timer, timer->seq_);
}

void TimingWheel::del_timer(TimerId tid)
{
    if (nullptr == tid.timer_)
        return;

    loop_->dispatch(std::bind(&TimingWheel::del_timer_loop, this, tid));
}

void TimingWheel::add_timer_loop(Timer* timer)
{
    timer->rotation_ = timer->interval_ / Count_Ticks;
    timer->slot_ = (cur_slot_ + timer->interval_) % Count_Ticks;
    wheel_[timer->slot_].insert(timer);
}

void TimingWheel::del_timer_loop(TimerId tid)
{
    auto it = wheel_[tid.timer_->slot_].find(tid.timer_);
    if (it != wheel_[tid.timer_->slot_].end()) {
        wheel_[tid.timer_->slot_].erase(it);
        delete tid.timer_; // (*it)
    }
}

void TimingWheel::handle_timeout(const boost::system::error_code& ec)
{
    if (ec)
        return;

    start();
    //do
    //{
    cur_slot_ = (cur_slot_ + 1) % Count_Ticks; // step one slot
    //LOGGER.write_log(LL_Trace, "TimingWheel handle timeout, slot : {}", cur_slot_);
    
    for (auto it = wheel_[cur_slot_].begin(); it != wheel_[cur_slot_].end(); )
    {
        Timer* timer = *it;
        if (timer->rotation_ > 0) {
            timer->rotation_--;
            ++it;
        }
        else { // timeout
            if (timer->task_)
                (timer->task_)();

            if (timer->repeat_)
                add_timer_loop(timer);
            else
                delete timer; // FIXME!

            it = wheel_[cur_slot_].erase(it);
        }
    }
    //} while (false);
}

