#ifndef _THREADGROUP_H_
#define _THREADGROUP_H_


#include <thread>
#include <vector>
#include <mutex>
#include <functional>

#include "block_queue.hpp"


namespace netlib
{

class ThreadWorker
{
public:
    using Task = std::function<void()>;

    explicit ThreadWorker(size_t max_task);
    ~ThreadWorker();

	ThreadWorker(const ThreadWorker&) = delete;
	ThreadWorker& operator=(const ThreadWorker&) = delete;

//	ThreadWorker(ThreadWorker&&) = default;
//  ThreadWorker& operator=(ThreadWorker&&) = default;

    //int start();
    int stop();
    bool push(const Task& task);

private:
    void run();

    block_queue<Task> tasks_;
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

    int start(size_t thread_num, size_t max_task = 0);
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