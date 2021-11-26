// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef NETLIB_TIMRID_H
#define NETLIB_TIMRID_H

#include <functional>


namespace netlib
{

typedef std::function<void()> TimerCallback;

class Timer;

class TimerId
{
public:
    TimerId() : timer_(nullptr), seq_(0) {}
    TimerId(Timer* timer, uint64_t seq) : timer_(timer), seq_(seq) {}

    ~TimerId() = default;
    TimerId(const TimerId&) = default;
    TimerId& operator=(const TimerId&) = default;

    friend class TimingWheel;

private:
    Timer* timer_;
    uint64_t seq_;
};

}

#endif // NETLIB_TIMRID_H