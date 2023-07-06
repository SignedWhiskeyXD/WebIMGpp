//
// Created by wsmrxd on 2023/7/6.
//

#ifndef WEBIMG_THREADINGQUEUE_H
#define WEBIMG_THREADINGQUEUE_H

#include <queue>
#include <semaphore>
#include <mutex>

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
        data.push(newElement);
        semaphore.release(1);
    }

    // 按右值引用，向容器尾部移动一个对象
    void enqueue(T&& newElement){
        QueueGuard guard(mtx);
        data.push(std::move(newElement));
        semaphore.release(1);
    }

    T wait_pop(){
        semaphore.acquire();
        QueueGuard guard(mtx);
        T ret = std::move(data.front());
        data.pop();
        return ret;
    }

private:
    using QueueGuard = std::lock_guard<std::mutex>;

    enum{
        SEM_LEAST_MAX_VALUE = 1000,
    };

    std::queue<T> data;

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
