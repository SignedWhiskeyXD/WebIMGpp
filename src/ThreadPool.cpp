//
// Created by wsmrxd on 2023/7/6.
//

#include "ThreadPool.h"

ThreadPool::ThreadPool(bool enableForceStop, uint16_t threadNum) :
        useForceStop(enableForceStop), threadNum(threadNum) {}

void ThreadPool::init() {
    while(workers.size() < threadNum){
        workers.emplace_back(&ThreadPool::workerTask, this);
    }
}

void ThreadPool::waitStop() noexcept {
    {
        Guard guard(mtx);
        stop = true;
    }
    cv_process_stop.notify_all();

    for(auto& t : workers)
        if(t.joinable()) t.join();
}

void ThreadPool::forceStop() noexcept {
    {
        Guard guard(mtx);
        stop = true;
        stopNow = true;
    }
    cv_process_stop.notify_all();

    for(auto& t : workers)
        if(t.joinable()) t.join();
}

bool ThreadPool::commit(ConnectionPtr connection) {
    if(stop) return false;
    {
        Guard guard(mtx);
        connections.push(std::move(connection));
    }
    cv_process_stop.notify_one();
    return true;
}

void ThreadPool::workerTask() {
    while(true){
        uGuard locker(mtx);
        cv_process_stop.wait(locker, [this]()->bool{
            return stop || !connections.empty();
        });
        
        if(stopNow || (stop && connections.empty())) return;

        ConnectionPtr newConnection = connections.front();
        connections.pop();
        locker.unlock();

        newConnection->handle();
    }
}
