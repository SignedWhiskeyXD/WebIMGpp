//
// Created by wsmrxd on 2023/7/6.
//

#include <catch2/catch_test_macros.hpp>
#include "ThreadingQueue.h"

TEST_CASE("QueuePush", "[ThreadingQueue]")
{
    ThreadingQueue<int> queue;
    const std::size_t nums = 1000;
    for(std::size_t i = 0; i < nums; ++i)
        queue.enqueue(std::make_shared<int>(114514));

    REQUIRE(queue.size() == nums);
}

TEST_CASE("QueuePop", "[ThreadingQueue]")
{
    ThreadingQueue<int> queue;
    const std::size_t nums = 1000;
    for(std::size_t i = 0; i < nums; ++i)
        queue.enqueue(std::make_shared<int>(114514));

    for(std::size_t i = 0; i < nums; ++i)
        queue.wait_pop();

    REQUIRE(queue.empty());
}

TEST_CASE("ThreadingPushPop", "[ThreadingQueue]")
{
    enum{
        PRODUCER_THREAD_NUM = 2,
        WORKER_THREAD_NUM = 8,
        PUSH_NUM_PER_PRODUCER = 500,
        POP_NUM_PER_WORKER = 100,
    };
    REQUIRE((PRODUCER_THREAD_NUM * PUSH_NUM_PER_PRODUCER - WORKER_THREAD_NUM * POP_NUM_PER_WORKER) >= 0);

    ThreadingQueue<int> queue;

    std::vector<std::thread> producers;
    while(producers.size() != PRODUCER_THREAD_NUM)
        producers.emplace_back([&queue](){
            for(int i = 0; i < PUSH_NUM_PER_PRODUCER; ++i)
                queue.enqueue(std::make_shared<int>(114514));
        });

    std::vector<std::thread> workers;
    while(workers.size() != WORKER_THREAD_NUM)
        workers.emplace_back([&queue](){
            for(int i = 0; i < POP_NUM_PER_WORKER; ++i)
                queue.wait_pop();
        });

    for(auto& t : producers)
        if(t.joinable()) t.join();

    for(auto& t : workers)
        if(t.joinable()) t.join();

    REQUIRE(queue.size() == (PRODUCER_THREAD_NUM * PUSH_NUM_PER_PRODUCER - WORKER_THREAD_NUM * POP_NUM_PER_WORKER));
}