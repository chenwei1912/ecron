#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_


#include <thread>
#include <vector>
#include <mutex>
#include <functional>
#include <atomic>

#include "block_queue.hpp"


namespace netlib
{

class ThreadWorker
{
public:
    explicit ThreadWorker(size_t max_task);
    ~ThreadWorker();

	ThreadWorker(const ThreadWorker&) = delete;
	ThreadWorker& operator=(const ThreadWorker&) = delete;

//	ThreadWorker(ThreadWorker&&) = default;
//  ThreadWorker& operator=(ThreadWorker&&) = default;

    //int start();
    int stop();

    template<typename F, typename... Args>
    bool push(F&& f, Args&&... args)
    {
        return queue_.push(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    }

private:
    void run();

    block_queue<std::function<void()>> queue_;
    std::thread thread_;

};

// pool of worker thread, each thread process a queue of tasks
class ThreadGroup
{
public:
    ThreadGroup();
    ~ThreadGroup();

	ThreadGroup(const ThreadGroup&) = delete;
	ThreadGroup& operator=(const ThreadGroup&) = delete;

	// move copy and assignment?

    int start(size_t thread_num, size_t max_task);
    int stop();

    // thread safe
    ThreadWorker* get_worker();

private:

    std::vector<ThreadWorker*> workers_;

    size_t next_;
    std::mutex mutex_;

};

}// namespace netlib

#endif // _THREADGROUP_H_