#include "ThreadGroup.h"


using namespace ecron;


ThreadWorker::ThreadWorker(size_t max_task)
    : thread_(std::bind(&ThreadWorker::run, this))
{
    tasks_.init(max_task);
}

ThreadWorker::~ThreadWorker()
{
    stop();
}

//int ThreadWorker::start(size_t max_task)
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
//    queue_.init(max_task);
//    return 0;
//}

int ThreadWorker::stop()
{
    if (thread_.get_id() == std::thread::id())
        return -1;

    tasks_.notify_exit();
    thread_.join();
    return 0;
}

bool ThreadWorker::push(const Task& task)
{
//    if (thread_.get_id() == std::thread::id())
//        return false;
    return tasks_.push(task);
}

// raw parameter for std::bind
//template<typename F, typename... Args>
//bool push(F&& f, Args&&... args)
//{
//    return queue_.push(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
//}

void ThreadWorker::run()
{
    while (true)
    {
        std::function<void()> task;
        if (!tasks_.pop(task))
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
    
