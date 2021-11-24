// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is an internal header file, you should not include this.

// https://blog.csdn.net/liushall/article/details/81316863
// https://www.fwqwd.com/12392.html
// multi wheel https://www.cnblogs.com/zhongwencool/p/timing_wheel.html

#ifndef NETLIB_TIMINGWHEEL_H
#define NETLIB_TIMINGWHEEL_H

#include "TimerId.h"


#include <unordered_set>
#include <memory>
#include <utility>

#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>





namespace netlib
{

class Timer
{
public:
    static int Timer_Count; // for calc seq_, atomic?
    static int generate_timerid();

    Timer(size_t interval, TimerCallback task, bool repeat)
        : interval_(interval)
        , task_(task)
        , repeat_(repeat)
        , seq_(generate_timerid())
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
    int seq_;
};

//typedef std::pair<Timer*, int> ActiveTimer;
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

}

#endif // NETLIB_TIMINGWHEEL_H
