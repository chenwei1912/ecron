#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_


#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>


namespace netlib
{

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// move disable?

    int start(size_t thread_num, size_t max_task, bool grow = false);
    int stop();

    template<typename F, typename... Args>
    bool append(F&& f, Args&&... args)
    {
        if (!running_)
            return false;

        //auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        //{
	    std::lock_guard<std::mutex> lock(mutex_);
	    if (tasks_.size() >= max_task_)
	        return false;

	    tasks_.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

	    if (idle_count_ < 1 && grow_)
	        add_thread(1);
	    
	    cond_.notify_one();
	    //}

        return true;
    }


private:
    void run();
    int add_thread(size_t num);

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::atomic_bool running_;
    std::atomic_int idle_count_;

    //size_t max_thread_; // max thread number
    size_t max_task_; // max task number in queue
    bool grow_;
};

}// namespace netlib

#endif // _THREADPOOL_H_
