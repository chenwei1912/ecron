// Copyright 2021, Ethan Chen.  All rights reserved.
// Author: Ethan Chen (ChenWei)
//
// This is a public header file, it must only include public header files.

#ifndef ECRON_LOCKFREEARRAYQUEUE_H
#define ECRON_LOCKFREEARRAYQUEUE_H


#include <vector>
#include <memory>



namespace ecron
{

template<typename T>
class LockFreeArrayQueue
{
public:
    explicit LockFreeArrayQueue()
                : max_count_(0)
                , head_(0)
                , tail_(0)
                , count_(0) {
    }

    ~LockFreeArrayQueue() {

    }
    
	LockFreeArrayQueue(const LockFreeArrayQueue&) = delete;
	LockFreeArrayQueue& operator=(const LockFreeArrayQueue&) = delete;

    // move ctor,move-assignment operator
	//LockFreeArrayQueue(LockFreeArrayQueue&&) = default;
    //LockFreeArrayQueue& operator=(LockFreeArrayQueue&&) = default;

    // fix size, not thread safe
    bool init(size_t max_count) {
        if (0 == max_count)
            return false;

        max_count_ = max_count;

        data_.reserve(max_count_);
        data_.clear();

        head_ = 0;
        tail_ = 0;
        count_ = 0;
        return true;
    }

	bool push(const T& item) {
	    if (full())
	        return false;

        // data_[tail_] = item;
        // tail_ = next_index;

        count_.fetch_add(1);
        return true;
	}

//    bool push(T&& item) {
//        return true;
//	}
	
	bool pop(T& item) {
	    if (empty())
	        return false;

        // item = data_[head_];
        // head_ = next_index;

        count_.fetch_sub(1);
        return true;
	}

	bool front(T& item) {

        return true;
	}

	bool back(T& item) {

        return true;
	}

	bool empty() {
	    // head_ == tail_;
        return (0 == count_);
	}

	bool full() {
        return (max_count_ <= count_);
	}

	size_t size() {
        return count_;
	}

	void clear() {

	}

private:
    inline size_t next_index(size_t idx) { return (idx + 1) % max_count_; }

    std::size_t max_count_;
    
    std::vector<std::atomic<T>> data_; // boost::circular_buffer<T>
    std::vector<std::atomic_int> flag_;

    std::atomic_size_t head_;
    std::atomic_size_t tail_;
    std::atomic_size_t count_;
    
};

}// namespace ecron

#endif // ECRON_LOCKFREEARRAYQUEUE_H