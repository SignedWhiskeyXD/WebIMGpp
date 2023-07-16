//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_THREADPOOL_H
#define WEBIMG_THREADPOOL_H

#include <unordered_set>
#include <queue>
#include <condition_variable>
#include "Connection.h"

class ThreadPool final {
    using Guard = std::lock_guard<std::mutex>;

    using uGuard = std::unique_lock<std::mutex>;

public:
    explicit ThreadPool(bool enableForceStop, uint16_t threadNum);

    ThreadPool() = delete;

    ThreadPool(const ThreadPool&) = delete;

    ThreadPool& operator=(const ThreadPool&) = delete;

    ~ThreadPool(){
        if(useForceStop)
            forceStop();
        else
            waitStop();
    }

    void init();

    // 将一个任务加入到任务队列
    bool commit(ConnectionPtr connection);

    // 等待所有任务完成后停止
    void waitStop() noexcept;

    // 不等待任务队列清空，完成手头工作后停止
    void forceStop() noexcept;

private:
    void workerTask();

    std::queue<ConnectionPtr> connections;

    std::vector<std::thread> workers;

    std::condition_variable cv_process_stop;

    std::mutex mtx;

    uint16_t threadNum;

    bool useForceStop;

    bool stop = false;

    bool stopNow = false;
};


#endif //WEBIMG_THREADPOOL_H
