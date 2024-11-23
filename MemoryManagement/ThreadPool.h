#pragma once

using namespace std;
#include <vector>
#include <thread>
#include <functional>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <mutex>
#include "CircularBuffer.hpp" 

class ThreadPool
{
private:
    vector<thread> workers;
    mutex tasksMutex;  // Mutex to protect shared data between workers
    condition_variable condition;  // Condition variable for workers
    atomic<bool> stopFlag;  // Flag to stop the worker threads
    CircularBuffer& buffer;  // CircularBuffer reference to fetch requests

    void worker();  // Worker function to process tasks

public:
    ThreadPool(size_t numThreads, CircularBuffer& cb);
    ~ThreadPool();
    void stop();
};

