#pragma once
#include <condition_variable>
#include <mutex>
#include <queue>
#include <complex>

template <typename T>
class StateUpdate{
    private:
        mutable std::mutex mtx;
        T data{};
        bool hasValue = false;
        std::vector<float> pcmBuffer;
        std::vector<std::complex<double>> pcmBufferComp;
        float maxSample;
        std::condition_variable cond;
    
    public:
        StateUpdate() = default;

        void set(T new_value){
            std::lock_guard<std::mutex> lock(mtx); // Acquire lock
            data = std::move(new_value); // Update recent state
            hasValue = true;
            cond.notify_one(); // Notifies a waiting thread
        }

        void set(T new_value, std::vector<float> pcmBuffer_, float maxMag_){
            std::lock_guard<std::mutex> lock(mtx); // Acquire lock
            data = std::move(new_value); // Update recent state
            hasValue = !pcmBuffer_.empty();
            pcmBuffer = pcmBuffer_;
            maxSample = maxMag_;
            cond.notify_one(); // Notifies a waiting thread
        }

        void set(T new_value, std::vector<std::complex<double>> pcmBuffer_, float maxMag_){
            std::lock_guard<std::mutex> lock(mtx); // Acquire lock
            data = std::move(new_value); // Update recent state
            hasValue = !pcmBuffer_.empty();
            pcmBufferComp = pcmBuffer_;
            maxSample = maxMag_;
            cond.notify_one(); // Notifies a waiting thread
        }

        void waitAndGet(T& value){
            std::unique_lock<std::mutex> lock(mtx); // Acquire lock
            cond.wait(lock, [this] { return hasValue;}); // Wait for state update is there is none
            value = data; // Retrieve state
            hasValue = false;
        }

        void waitAndGet(T& value, std::vector<float>& result, float& maxMag){
            std::unique_lock<std::mutex> lock(mtx); // Acquire lock
            cond.wait(lock, [this] { return hasValue;}); // Wait for state update is there is none
            value = data; // Retrieve state
            hasValue = false;
            result = pcmBuffer;
            maxMag = maxSample;
        }

        void waitAndGet(T& value, std::vector<std::complex<double>>& result, float& maxMag){
            std::unique_lock<std::mutex> lock(mtx); // Acquire lock
            cond.wait(lock, [this] { return hasValue;}); // Wait for state update is there is none
            value = data; // Retrieve state
            hasValue = false;
            result = pcmBuffer;
            maxMag = maxSample;
        }

        bool empty() const{
            std::lock_guard<std::mutex> lcok(mtx);
            return !hasValue;
        }

        ~StateUpdate() = default;
};