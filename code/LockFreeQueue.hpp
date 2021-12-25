#ifndef ECRON_LOCKFREEQUEUE_H
#define ECRON_LOCKFREEQUEUE_H


#include <atomic>


namespace ecron
{

// MPMC - multi-producer multi-consumer
template<typename T>
class LockFreeQueue
{
public:

    struct node
    {
        node(const T& data)
            : data_(data)
            , next_(nullptr) {}

        T data_;
        std::atomic<node*> next_;
    };

	LockFreeQueue()
        : head_(nullptr)
        , tail_(nullptr)
        , count_(0)
        , max_count_(0) {
	}

	~LockFreeQueue() {
        clear();
	}

	LockFreeQueue(const LockFreeQueue&) = delete;
	LockFreeQueue& operator=(const LockFreeQueue&) = delete;

	//LockFreeQueue(LockFreeQueue&&) = default;
    //LockFreeQueue& operator=(LockFreeQueue&&) = default;

    // no size limit, not thread safe
    bool init(size_t max_count = 0) {
        max_count_ = max_count;
        count_ = 0;
        clear();

        node* sentry = new node(T()); // queue's sentry
        head_ = sentry;
        tail_ = sentry;
        return true;
    }

	bool push(const T& item) {
	    if (full())
	        return false;

        // tail_->next_ = new_node;
        // tail_ = new_node;

	    node* new_node = new node(item);
	    node* old_tail = nullptr;
	    node* next_node = nullptr;
        while (true) {
            old_tail = tail_;
            next_node = old_tail->next_;

            // tail_ is updated in other thread
            if (old_tail != tail_)
                continue;

            // tail_ is updated in other thread
            if (nullptr != next_node) {
                tail_.compare_exchange_weak(old_tail, next_node, 
                                            std::memory_order_release, 
                                            std::memory_order_relaxed);
                continue;
            }

            // update old_tail->next_
            if (old_tail->next_.compare_exchange_weak(next_node, new_node, 
                                                    std::memory_order_release, 
                                                    std::memory_order_relaxed))
                break;
        }

        tail_.compare_exchange_weak(old_tail, new_node, 
                                    std::memory_order_release, 
                                    std::memory_order_relaxed);
        count_.fetch_add(1);
        return true;
	}

//    bool push(T&& item) {
//        return true;
//	}
	
	bool pop(T& item) {
	    if (empty())
	        return false;

        // item = head_->next_->data_;
        // head_ = head_->next_;

	    node* old_head = nullptr;
	    node* old_tail = nullptr;
	    node* next_node = nullptr;
        while (true) {
            old_head = head_;
            old_tail = tail_;
            next_node = old_head->next_;

            // head_ is updated in other thread
            if (old_head != head_)
                continue;

            // empty queue
            if (old_head == old_tail && nullptr == next_node)
                return false;

            // in other push thread, insert new node(next_node)
            // but tail_ is not updated
            if (old_head == old_tail && nullptr != next_node) {
                tail_.compare_exchange_weak(old_tail, next_node, 
                                            std::memory_order_release, 
                                            std::memory_order_relaxed);
                continue;
            }

            item = next_node->data_;
            if (head_.compare_exchange_weak(old_head, next_node, 
                                            std::memory_order_release, 
                                            std::memory_order_relaxed))
                break;
        }

        delete old_head;
        count_.fetch_sub(1);
        return true;
	}

	bool front(T& item) {
	    node* node = head_->next_;
	    if (nullptr == node)
	        return false;

	    item = node->data_;
        return true;
	}

	bool back(T& item) {
	    node* node = tail_;
	    if (nullptr == node)
	        return false;

	    item = node->data_;
        return true;
	}

	bool empty() {
	    // head_ == tail_;
        return (0 == count_);
	}

	bool full() {
        return (max_count_ > 0 && max_count_ <= count_);
	}

	size_t size() {
        return count_;
	}

	void clear() {
        node* cur = head_;
        while (nullptr != cur) {
            node* next = cur->next_;
            delete cur;
            cur = next;
        }
	}


private:
    std::atomic<node*> head_;
    std::atomic<node*> tail_;

    std::atomic_size_t count_;
    std::size_t max_count_;
};

}// namespace ecron

#endif // ECRON_LOCKFREEQUEUE_H