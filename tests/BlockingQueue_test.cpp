#include "catch.hpp"

#include "BlockingQueue.hpp"

#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <unordered_map>

#include <iostream>



TEST_CASE("test BlockingQueue", "[BlockingQueue]") {

    ecron::BlockingQueue<int> queue;
    queue.init();

    std::vector<std::thread> pthreads;
    std::vector<std::thread> cthreads;
    const std::size_t pn = 100;
    const std::size_t cn = 100;

    const int batch = 100;
    const int num = 100;

    std::atomic<int> cnt;
    std::atomic<int> pop_cnt(0);
    std::atomic<int> push_cnt(0);
    std::mutex mx;
    std::condition_variable cond;
    bool running = false;

    std::unordered_map<int, int> counts[cn];

    for (size_t i = 0; i < pn; ++i)
    {
        pthreads.emplace_back([&]{
            {
                std::unique_lock<std::mutex> lock(mx);
                cond.wait(lock, [&]{ return running; });
            }
            for (int i = 0; i < batch; ++i)
                for (int j = 1; j <= num; ++j) {
                    while (true) {
                        if (queue.push(j)) {
                            push_cnt.fetch_add(1);
                            break;
                        }
                        else
                            std::cout << "thread: " << i << " push failed: " << j << std::endl;
                    }
                }

            ++cnt;
        });
    }
    for (size_t i = 0; i < cn; ++i)
    {
        cthreads.emplace_back([&, i](){
            std::unordered_map<int, int>& count = counts[i];
            {
                std::unique_lock<std::mutex> lock(mx);
                cond.wait(lock, [&]{ return running; });
            }
            while (true) {
                int val = 0;
                bool flag = queue.pop(val);
                if (flag) {
                    if (val > num)
                        std::cout << "range thread: " << i << " pop: " << val << std::endl;
                    ++count[val];
                    pop_cnt.fetch_add(1);
                }
                else //if (cnt == pn)
                    break;
            }
            //std::cout << "thread: " << i << " size: " << count.size() << std::endl;
        });
    }

    {
        std::lock_guard<std::mutex> lock(mx);
        running = true;
        cond.notify_all();
    }
    
    for (auto& item : pthreads)
        if (item.joinable())
            item.join();
    pthreads.clear();

    for (auto& item : cthreads)
        if (item.joinable())
            item.join();
    cthreads.clear();

    REQUIRE(cnt == pn);
    REQUIRE(queue.empty());
    REQUIRE(push_cnt == (num * batch * pn));
    REQUIRE(pop_cnt == push_cnt);

    std::unordered_map<int, int> res;
    for (size_t i = 0; i < cn; ++i) {
    //for (auto& count : counts) {
        for(auto& item : counts[i]) {
            if (item.first <= 0 || item.first > num)
                std::cout << "count: "<< i << " " << item.first << " " << item.second << std::endl;
            res[item.first] += item.second;
        }
    }

    int total = 0;
    for (auto& item : res) {
        total += item.second;
        REQUIRE(item.first > 0);
        REQUIRE(item.first <= num);
        REQUIRE(item.second == batch * pn);
    }
    REQUIRE(total == num * batch * pn);

}


