#ifndef HEAP_MANAGER_HPP
#define HEAP_MANAGER_HPP

#include "MemoryManager.h"
#include <mutex>

#pragma once

class HeapManager {
private:
    MemoryManager memManager;

    mutable std::mutex mtx;

public:
    HeapManager(size_t initialSegmentSize, size_t initialSegmentsNumber);

    bool allocate_memory(int size);

    bool free_memory(int address);

    void printMemory();
};

#endif // !HEAP_MANAGER_HPP
