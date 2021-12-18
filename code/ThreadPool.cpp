#include "ThreadPool.h"


#define POOL_MAXTHREAD_NUM 16


using namespace netlib;


ThreadPool::ThreadPool()
    : running_(false)
    , idle_count_(0)
    , max_task_(0)
    , grow_(false)
{
}

ThreadPool::~ThreadPool()
{
    stop();
}

int ThreadPool::start(size_t thread_num, size_t max_task, bool grow)
{
    if (thread_num < 1 || running_)
        return -1;

    max_task_ = max_task;
    grow_ = grow;
    running_ = true;
    add_thread(thread_num);
    return 0;
}

int ThreadPool::stop()
{
    if (!running_)
        return -1;

    {
        std::lock_guard<std::mutex> lock(mutex_);
        running_ = false;
        cond_.notify_all();
    }

    for (auto& item : threads_)
    {
        if (item.joinable()) // item.get_id() != std::thread::id()
            item.join();
    }
    threads_.clear();

    return 0;
}

bool ThreadPool::append(const Task& task)
{
    if (!running_)
        return false;

    std::lock_guard<std::mutex> lock(mutex_);
    if (max_task_ > 0 && tasks_.size() >= max_task_)
        return false;

    tasks_.push(task);

    if (grow_ && idle_count_ < 1)
        add_thread(1);
    
    cond_.notify_one();
    return true;
}

// raw parameter for std::bind
/*template<typename F, typename... Args>
bool append(F&& f, Args&&... args)
{
    if (!running_)
        return false;

    //auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    std::lock_guard<std::mutex> lock(mutex_);
    if (tasks_.size() >= max_task_)
        return false;

    tasks_.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    if (idle_count_ < 1 && grow_)
        add_thread(1);
    
    cond_.notify_one();
    return true;
}*/
    
void ThreadPool::run()
{
    while (true)
    {
        Task task;
        {
    	    std::unique_lock<std::mutex> lock(mutex_);
    	    //while (tasks_.empty() && running_)
    	    //    cond_.wait(lock);
    	    cond_.wait(lock, [this](){ return (!tasks_.empty() || !running_); });

            if (!running_) // not exec remain tasks
                break;

            task = std::move(tasks_.front());
            tasks_.pop();
        }

        if (task)
        {
            idle_count_--;
            task();
            idle_count_++;
        }
    }
}

int ThreadPool::add_thread(size_t num)
{
    for (size_t i = 0; i < num; ++i)
    {
        threads_.emplace_back(std::bind(&ThreadPool::run, this));
        idle_count_++;
    }
    return 0;
}
    