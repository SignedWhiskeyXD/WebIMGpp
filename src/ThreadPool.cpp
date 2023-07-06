//
// Created by wsmrxd on 2023/7/6.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(int workerNum, bool enableForceStop):
        useForceStop(enableForceStop)
{
    while(workers.size() < workerNum){
        workers.emplace_back([this](){
            while(true){
                if(stop && taskQueue.empty() || stopNow) return;

                auto task = this->taskQueue.wait_pop();
                if(task == nullptr) continue;
                // TODO: 处理对获取连接的逻辑
                task->info();
            }
        });
    }
}

void ThreadPool::waitStop() noexcept {
    stop = true;
    taskQueue.releaseFor(workers.size());
    for(auto& t : workers)
        if(t.joinable()) t.join();
}

void ThreadPool::forceStop() noexcept {
    stop = true;
    stopNow = true;
    taskQueue.releaseFor(workers.size());
    for(auto& t : workers)
        if(t.joinable()) t.join();
}

bool ThreadPool::commit(std::shared_ptr<Connection> connection) {
    if(stop) return false;
    taskQueue.enqueue(std::move(connection));
    return true;
}