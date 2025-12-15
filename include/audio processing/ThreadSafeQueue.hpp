#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>

template <typename T>
class ThreadSafeQueue{
    private:
        mutable std::mutex mtx;
        std::queue<T> data;
        std::condition_variable cond;
    
    public:
        ThreadSafeQueue() = default;

        void push(T new_value){
            std::lock_guard<std::mutex> lock(mtx); // Acquire lock
            data.push(std::move(new_value)); // Add value to queue
            cond.notify_one(); // Notifies a waiting thread
        }

        void waitAndPop(T& value){
            std::unique_lock<std::mutex> lock(mtx); // Acquire lock
            cond.wait(lock, [this] { return !data.empty();}); // Wait until queue is not empty
            value = std::move(data.front()); // Retrieve value from queue
            data.pop();
        }

        bool empty() const{
            std::lock_guard<std::mutex> lcok(mtx);
            return data.empty();
        }

        ~ThreadSafeQueue() = default;
};