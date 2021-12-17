#include "ThreadGroup.h"


using namespace netlib;


ThreadWorker::ThreadWorker(size_t max_task)
    : queue_(max_task)
    , thread_(std::bind(&ThreadWorker::run, this))
{
}

ThreadWorker::~ThreadWorker()
{
    stop();
}

//int ThreadWorker::start()
//{
//    try
//    {
//        thread_.reset(new std::thread(std::bind(&ThreadWorker::run, this)));
//    }
//    catch (std::bad_alloc&)
//    {
//        // log
//        return -1;
//    }

//    return 0;
//}

int ThreadWorker::stop()
{
    if (thread_.get_id() == std::thread::id())
        return -1;

    // joinable
    queue_.notify_exit();
    thread_.join();
    return 0;
}

void ThreadWorker::run()
{
    while (true)
    {
        std::function<void()> task;
        if (!queue_.pop(task))
            break;

        if (task)
        {
            // perf timing
            task();
            // perf timing
        }
    }
}


ThreadGroup::ThreadGroup()
    //: running_(false)
    : next_(0)
{
}

ThreadGroup::~ThreadGroup()
{
    stop();
}

int ThreadGroup::start(size_t thread_num, size_t max_task)
{
    if (thread_num < 1 || max_task < 1 || !workers_.empty())
        return -1;

    for (size_t i = 0; i < thread_num; ++i)
    {
        workers_.push_back(new ThreadWorker(100000));
    }

    next_ = 0;
    return 0;
}

int ThreadGroup::stop()
{
    if (workers_.empty())
        return -1;

    for (auto it = workers_.begin(); it != workers_.end(); ++it)
    {
        (*it)->stop();
        delete (*it);
    }

    workers_.clear();
    next_ = 0;
    return 0;
}

ThreadWorker* ThreadGroup::get_worker()
{
    if (workers_.empty())
        return nullptr;

    ThreadWorker* worker = nullptr;
    std::lock_guard<std::mutex> lock(mutex_);
    worker = workers_[next_];
    ++next_;
    next_ %= workers_.size(); // round-robin
    return worker;
}
    
