// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is an internal header file, you should not include this.

// https://blog.csdn.net/liushall/article/details/81316863
// https://www.fwqwd.com/12392.html
// multi wheel https://www.cnblogs.com/zhongwencool/p/timing_wheel.html

#ifndef ECRON_TIMINGWHEEL_H
#define ECRON_TIMINGWHEEL_H

#include "TimerId.h"


#include <unordered_set>
#include <memory>
#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>





namespace ecron
{
namespace net
{

class Timer
{
public:
    //static int Timer_Count; // for calc seq_, atomic?
    static uint64_t generate_timerid();

    Timer(size_t interval, TimerCallback task, bool repeat)
        : interval_(interval)
        , task_(task)
        , repeat_(repeat)
        , seq_(num_created_.fetch_add(1))
    {}

    //Timer() = default;
    ~Timer() = default;

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

//private:
    size_t interval_; // timeout - unit second
    TimerCallback task_;
    bool repeat_;
    size_t rotation_; // wheel circle count, no use multiple wheels
    size_t slot_; // slot positon
    uint64_t seq_;

    static std::atomic<uint64_t> num_created_;
};


typedef std::pair<Timer*, uint64_t> TimerItem;
struct pair_hash // user define hash method
{
    template <class T1, class T2>
    std::size_t operator() (const std::pair<T1, T2>& pair) const {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};
//typedef std::unordered_set<TimerItem, pair_hash> Bucket;

typedef std::unordered_set<Timer*> Bucket;
//typedef std::unordered_set<std::unique_ptr<Timer>> Bucket;

class EventLoop;

class TimingWheel
{
public:
    static const size_t Count_Ticks = 60;
    static const size_t Interval_Tick = 1; // second

    explicit TimingWheel(EventLoop* loop);
    ~TimingWheel();

    TimingWheel(const TimingWheel&) = delete;
    TimingWheel& operator=(const TimingWheel&) = delete;

    void start();
    void stop();

    // thread safe?
    TimerId add_timer(size_t interval, TimerCallback task, bool repeat = false);
    void del_timer(TimerId tid);

private:
    void add_timer_loop(Timer* timer);
    void del_timer_loop(TimerId tid);

    void handle_timeout(const boost::system::error_code& ec);

    EventLoop* loop_;
    boost::asio::steady_timer timer_;

    // timing wheel
    // prefer to use std::unique_ptr<Timer>
    std::array<Bucket, Count_Ticks> wheel_;
    size_t cur_slot_; // current slot index

};

}// namespace net
}// namespace ecron

#endif // ECRON_TIMINGWHEEL_H