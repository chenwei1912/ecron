#ifndef ECRON_THREADPOOL_H
#define ECRON_THREADPOOL_H


#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

#include "BlockingQueue.hpp"


namespace ecron
{

class ThreadPool
{
public:
    using Task = std::function<void()>;

    ThreadPool();
    ~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	// move disable?

    int start(size_t thread_num, size_t max_task = 0, bool grow = false);
    int stop();

    bool append(const Task& task);

private:
    void run();
    int add_thread(size_t num);

    std::vector<std::thread> threads_;
    BlockingQueue<Task> tasks_;
    //std::atomic_bool running_;

    //size_t max_thread_; // max thread number
    size_t max_task_; // max task number in queue
    std::atomic_int idle_count_;
    bool grow_;
};

}// namespace ecron

#endif // ECRON_THREADPOOL_H