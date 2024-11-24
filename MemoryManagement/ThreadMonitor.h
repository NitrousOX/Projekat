// ThreadMonitor.hpp
#pragma once
#include <atomic>
#include <iostream>

class ThreadMonitor {
private:
    std::atomic<int> activeThreads;  // Track the number of active threads

public:
    ThreadMonitor() : activeThreads(0) {}

    // Called when a thread starts
    void threadStarted() {
        activeThreads++;
        std::cout << "Thread started. Active threads: " << activeThreads.load() << std::endl;
    }

    // Called when a thread finishes
    void threadFinished() {
        activeThreads--;
        std::cout << "Thread finished. Active threads: " << activeThreads.load() << std::endl;
    }

    // Called to display the current number of active threads
    void displayActiveThreads() {
        std::cout << "Currently active threads: " << activeThreads.load() << std::endl;
    }
};
