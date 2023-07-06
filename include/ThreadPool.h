//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_THREADPOOL_H
#define WEBIMG_THREADPOOL_H

#include <functional>
#include <condition_variable>
#include "ThreadingQueue.h"

class ThreadPool final {
public:
    explicit ThreadPool(int workerNum, bool enableForceStop = true);

    ThreadPool() = delete;

    ThreadPool(const ThreadPool&) = delete;

    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool(){
        if(useForceStop)
            forceStop();
        else
            waitStop();
    }

    // 将一个任务加入到任务队列
    template<class Func, class... Args>
    bool commit(Func&& func, Args&&... args)
    {
        if(stop) return false;
        auto task = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
        taskQueue.enqueue(task);
        return true;
    }

    // 等待所有任务完成后停止
    void waitStop() noexcept;

    // 不等待任务队列清空，完成手头工作后停止
    void forceStop() noexcept;

private:
    ThreadingQueue<std::function<void()>> taskQueue;

    std::vector<std::thread> workers;

    std::condition_variable conditionStop;

    std::mutex mutexStop;

    bool useForceStop;

    bool stop = false;

    bool stopNow = false;
};

inline ThreadPool::ThreadPool(int workerNum, bool enableForceStop):
    useForceStop(enableForceStop)
{
    while(workers.size() < workerNum){
        workers.emplace_back([this](){
            while(true){
                if(stop && taskQueue.empty() || stopNow) return;

                auto task = this->taskQueue.wait_pop();
                if(task == nullptr) continue;
                (*task)();
            }
        });
    }
}

// TODO: 处理任意参数列表的任务


inline void ThreadPool::waitStop() noexcept {
    stop = true;
    taskQueue.releaseFor(workers.size());
    for(auto& t : workers)
        if(t.joinable()) t.join();
}

inline void ThreadPool::forceStop() noexcept {
    stop = true;
    stopNow = true;
    taskQueue.releaseFor(workers.size());
    for(auto& t : workers)
        if(t.joinable()) t.join();
}


#endif //WEBIMG_THREADPOOL_H
