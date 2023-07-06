//
// Created by wsmrxd on 2023/7/6.
//

#include <catch2/catch_test_macros.hpp>
#include "ThreadPool.h"

class Foo{
public:
    explicit Foo(int v): val(v) {}

    void addToThis(int rhs){
        val += rhs;
    }

    int getVal() const{
        return this->val;
    }

private:
    int val = 0;
};

TEST_CASE("commitMethods", "[ThreadPool]")
{
    ThreadPool threadPool(2);
    Foo foo(0);

    threadPool.commit([&foo](){
        foo.addToThis(114514);
    });

    threadPool.waitStop();
    REQUIRE(foo.getVal() == 114514);
}

TEST_CASE("commitTasks", "[ThreadPool]")
{
    const int TASK_NUM = 10;

    ThreadPool threadPool(8);

    int sum = 0;

    for(int i = 0; i < TASK_NUM; ++i)
        threadPool.commit([&sum](){
            static std::mutex mtx;
            int local = 0;

            for(int i = 0; i < 1000; ++i)
                ++local;

            mtx.lock();
            sum += local;
            mtx.unlock();
        });

    threadPool.waitStop();

    REQUIRE(sum == TASK_NUM * 1000);
}