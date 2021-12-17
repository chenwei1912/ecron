#include "catch.hpp"


#include "ThreadPool.h"
#include "ThreadGroup.h"

#include <iostream>
#include <thread>
#include <chrono>


//bool thread_is_active(const std::thread::id& thread_id) { 
//    return thread_id != std::thread::id();
//}

class TestRunner
{
public:
    TestRunner() {}
    ~TestRunner() {}

    void run(int i) {
        //++i;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
};

void run_free(int i) {
    //++i;
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
}

void myadd(int a, int b)
{
    int r = a + b;
    std::cout << "myadd - " << r << std::endl;
    //++r;
}

TEST_CASE("ThreadPool test case", "[ThreadPool]") {

    netlib::ThreadPool pool;
    bool ret = pool.start(4, 100000);
    REQUIRE(0 == ret);

    for (int i = 0; i < 8; ++i) {
        pool.append(myadd, i, i + 1);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "wait thread pool stop" << std::endl;
    pool.stop();

}

TEST_CASE("ThreadGroup test case", "[ThreadGroup]") {

    netlib::ThreadGroup group;
    int ret = group.start(4, 100000);
    REQUIRE(0 == ret);

    for (int i = 0; i < 8; ++i) {
        netlib::ThreadWorker* worker = group.get_worker();
        std::cout << "worker value : " << worker << std::endl;
        worker->push(myadd, i, i + 1);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    std::cout << "wait thread group stop" << std::endl;
    group.stop();

}

