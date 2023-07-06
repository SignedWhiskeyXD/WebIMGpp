//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_THREADPOOL_H
#define WEBIMG_THREADPOOL_H

#include <functional>
#include "ThreadingQueue.h"
#include "Connection.h"

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
    bool commit(std::shared_ptr<Connection> connection);

    // 等待所有任务完成后停止
    void waitStop() noexcept;

    // 不等待任务队列清空，完成手头工作后停止
    void forceStop() noexcept;

private:
    ThreadingQueue<Connection> taskQueue;

    std::vector<std::thread> workers;

    bool useForceStop;

    bool stop = false;

    bool stopNow = false;
};


#endif //WEBIMG_THREADPOOL_H
