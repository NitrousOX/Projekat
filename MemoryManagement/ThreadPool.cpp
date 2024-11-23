#include "ThreadPool.h"
#include <iostream>

ThreadPool::ThreadPool(size_t numThreads, CircularBuffer& cb) : stopFlag(false), buffer(cb) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::worker, this);
    }
}

ThreadPool::~ThreadPool() {
    stop();
    for (std::thread& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::worker() {
    while (!stopFlag.load()) {
        ClientRequest request; 


        {
            std::unique_lock<std::mutex> lock(tasksMutex);

            condition.wait(lock, [this] { return stopFlag.load() || !buffer.isEmpty(); });

            if (stopFlag.load() && buffer.isEmpty()) {
                return;
            }

            if (!buffer.remove(request)) {
                continue;  
            }
        }
        cout << "Processing request: " << request << endl;
    }
}

void ThreadPool::stop() {
    stopFlag.store(true); 
    condition.notify_all();  
}

