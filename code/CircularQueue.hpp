#ifndef ECRON_CIRCULARQUEUE_H
#define ECRON_CIRCULARQUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <chrono>


namespace ecron
{

template<typename T>
class CircularQueue
{
public:
    using item_type = T;

	CircularQueue()
        : max_count_(0)
        , head_(0)
        , tail_(0) {
	}

	~CircularQueue() {
        //clear(); // thread safe?
	}

	CircularQueue(const BlockingQueue&) = delete;
	CircularQueue& operator=(const BlockingQueue&) = delete;

    //CircularQueue(CircularQueue&&) = default;
    //CircularQueue& operator=(CircularQueue&&) = default;

    // not thread safe
    bool init(size_t max_count) {
        max_count_ = max_count + 1; // one item is reserved as marker for full
        head_ = 0;
        tail_ = 0;
        vector_.reserve(max_count_);
        return true;
    }

	bool push(const T& item) {
	    if (full()) {
            head_ = next_index(head_); // overwrite the oldest element
	    }

	    vector_[tail_] = item;
	    tail_ = next_index(tail_);
        return true;
	}

    bool push(T&& item) {
	    if (full()) {
            head_ = next_index(head_); // overwrite the oldest element
	    }

	    vector_[tail_] = std::move(item);
	    tail_ = next_index(tail_);
        return true;
	}
	
	bool pop(T& item) {
	    if (empty()) {
            return false;
	    }

        item = std::move(vector_[head_]);
        head_ = next_index(head_);
        return true;
	}

	bool front(T& item) {
	    if (empty()) {
            return false;
	    }

        item = vector_[head_];
        return true;
	}

	bool back(T& item) {
	    if (empty()) {
            return false;
	    }

        item = vector_[prev_index(tail_)];
        return true;
	}

	bool empty() {
        return head_ == tail_;
	}

	bool full() {
        return head_ == next_index(tail_);
	}

	size_t size() {
        return (tail_ - head_ + max_count_) % max_count_;
	}


private:
    inline size_t next_index(size_t idx) {
        return (idx + 1) % max_count_;
    }
    inline size_t prev_index(size_t idx) {
        return (idx + max_count_ - 1) % max_count_
    }

    std::vector<T> vector_;

    size_t max_count_;
    size_t head_;
    size_t tail_;

};

}// namespace ecron

#endif // ECRON_CIRCULARQUEUE_H