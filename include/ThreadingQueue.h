//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_THREADINGQUEUE_H
#define WEBIMG_THREADINGQUEUE_H

#include <queue>
#include <semaphore>
#include <mutex>
#include <memory>

template<class T>
class ThreadingQueue {
public:
    ThreadingQueue(): semaphore(0) {}

    ThreadingQueue(const ThreadingQueue&) = delete;

    ThreadingQueue& operator=(const ThreadingQueue&) = delete;

    std::size_t size() const;

    bool empty() const;

    // 按左值引用，向队列尾部拷贝一个对象
    void enqueue(const T& newElement){
        QueueGuard guard(mtx);
        data.push(std::make_unique<T>(newElement));
        semaphore.release(1);
    }

    // 按右值引用，向容器尾部移动一个对象
    void enqueue(T&& newElement){
        QueueGuard guard(mtx);
        data.push(std::make_unique<T>(std::move(newElement)));
        semaphore.release(1);
    }

    std::unique_ptr<T> wait_pop(){
        semaphore.acquire();
        QueueGuard guard(mtx);
        if(data.empty()) return nullptr;
        std::unique_ptr<T> ret = std::move(data.front());
        data.pop();
        return ret;
    }

    std::unique_ptr<T> try_pop(std::size_t msecs){
        bool isAcquired = semaphore.try_acquire_for(std::chrono::milliseconds(msecs));
        if(!isAcquired) return nullptr;

        QueueGuard guard(mtx);
        std::unique_ptr<T> ret = std::move(data.front());
        data.pop();
        return ret;
    }

    void releaseFor(std::size_t count){
        semaphore.release(count);
    }

private:
    using QueueGuard = std::lock_guard<std::mutex>;

    enum{
        SEM_LEAST_MAX_VALUE = 1000,
    };

    std::queue<std::unique_ptr<T>> data;

    std::counting_semaphore<SEM_LEAST_MAX_VALUE> semaphore;

    mutable std::mutex mtx;

};

template<class T>
inline std::size_t ThreadingQueue<T>::size() const {
    QueueGuard guard(mtx);
    return data.size();
}

template<class T>
inline bool ThreadingQueue<T>::empty() const {
    QueueGuard guard(mtx);
    return data.empty();
}


#endif //WEBIMG_THREADINGQUEUE_H
