#ifndef _BLOCK_QUEUE_H_
#define _BLOCK_QUEUE_H_

#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>


namespace netlib
{

template<typename T>
class block_queue
{
public:
	block_queue()
        : max_count_(0)
        , exit_(false) {
	}

	~block_queue() {
        //clear(); // thread safe?
	}

	block_queue(const block_queue&) = delete;
	block_queue& operator=(const block_queue&) = delete;

//	block_queue(block_queue&&) = default;
//    block_queue& operator=(block_queue&&) = default;

    // not thread safe
    bool init(size_t max_count = 0) {
        max_count_ = max_count;
        exit_ = false;
        queue_.clear();
        return true;
    }

	bool push(const T& item) {
	    std::lock_guard<std::mutex> lock(mutex_);
	    if (max_count_ > 0 && queue_.size() >= max_count_)
	        return false;

	    queue_.push_back(item);
        cond_.notify_one();
        return true;
	}

    bool push(T&& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (max_count_ > 0 && queue_.size() >= max_count_)
            return false;

        queue_.push_back(std::move(item));
        cond_.notify_one();
        return true;
	}
	
	bool pop(T& item) {
	    std::unique_lock<std::mutex> lock(mutex_);
	    //while (queue_.empty() && !exit_)
	    //    cond_.wait(lock);
	    cond_.wait(lock, [this](){ return (!queue_.empty() || exit_); });

	    if (exit_)
	        return false;

        item = std::move(queue_.front());
        queue_.pop_front();
        return true;
	}
	
//	bool pop(T& item, int ms_timeout) {
//	    // check ms_timeout?

//	    std::unique_lock<std::mutex> lock(mutex_);
//	    while (queue_.empty() && !exit_)
//	        if (std::cv_status::timeout 
//                    == cond_.wait_for(lock, std::chrono::milliseconds(ms_timeout)))
//	            return false; // timeout

//        if (exit_)
//            return false;

//        item = queue_.front();
//        queue_.pop_front();
//        return true;
//	}

	bool front(T& item) {
	    std::lock_guard<std::mutex> lock(mutex_);
	    if (queue_.empty())
	        return false;
	    item = queue_.front();
        return true;
	}

	bool back(T& item) {
	    std::lock_guard<std::mutex> lock(mutex_);
	    if (queue_.empty())
	        return false;
	    item = queue_.back();
        return true;
	}

	bool empty() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
	}

	bool full() {
        std::lock_guard<std::mutex> lock(mutex_);
        return (queue_.size() == max_count_);
	}

	size_t size() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
	}

//	void clear() {
//        std::lock_guard<std::mutex> lock(mutex_);
//        queue_.clear();
//	}

	void notify_exit() {
        std::lock_guard<std::mutex> lock(mutex_);
        exit_ = true;
        cond_.notify_all();
	}


private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
    size_t max_count_;
    bool exit_;
};

}// namespace netlib

#endif // _BLOCK_QUEUE_H_