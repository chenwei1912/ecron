#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_


#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>


class ThreadPool
{
public:
    ThreadPool()
        : stop_(true)
        , max_count_(0)
    {}
    
    ~ThreadPool() { stop(); }

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// move disable?

    bool start(size_t thread_number, size_t max_request)
    {
        if (thread_number < 1 || max_request < 1)
            return false;

        stop_ = false;
        max_count_ = max_request;
        
        for (size_t i = 0; i < thread_number; ++i)
        {
            threads_.emplace_back(std::bind(&ThreadPool::run, this));
        }

        return true;
    }

    bool stop()
    {
        if (stop_)
            return true;

        {
            std::lock_guard<std::mutex> lock(mutex_);
            stop_ = true;
            cond_.notify_all();
        }

        for (auto& thd : threads_)
            thd.join();

        threads_.clear();
        return true;
    }

    template<typename F, typename... Args>
    bool append(F&& f, Args&&... args)
    {
        //auto task = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

        //{
	    std::lock_guard<std::mutex> lock(mutex_);
	    if (tasks_.size() >= max_count_)
	        return false;

	    tasks_.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
	    cond_.notify_one();
	    //}

        return true;
    }


private:
    void run()
    {
        while (true)
        {
            std::function<void()> task;
            {
        	    std::unique_lock<std::mutex> lock(mutex_);
        	    //while (tasks_.empty() && !stop_)
        	    //    cond_.wait(lock);
        	    cond_.wait(lock, [this](){ return (!tasks_.empty() || stop_); });

                if (stop_)
                    return;

                task = std::move(tasks_.front());
                tasks_.pop();
            }
            task(); // run task
        }
    }

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    std::mutex mutex_;
    std::condition_variable cond_;
    bool stop_;

    //int thread_number_; // max thread number
    size_t max_count_; // max task number in queue
};

#endif // _THREADPOOL_H_