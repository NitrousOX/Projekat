#include "HeapManager.hpp"

HeapManager::HeapManager(size_t initialSegmentSize, size_t initialSegmentsNumber = 5)
    : memManager(initialSegmentSize, initialSegmentsNumber),  // Default constructor for MemoryManager
    headFreeMemory(nullptr),
    tailFreeMemory(nullptr),
    freeSegments(0) {}



void* HeapManager::allocate_memory(int size) {
    //TODO
    return nullptr;
}

void HeapManager::free_memory(void* address) {
   //TODO
}