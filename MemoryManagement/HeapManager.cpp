#include "HeapManager.hpp"
#include <cstdlib>
#include <iostream>

HeapManager::HeapManager(size_t initialSegmentSize, size_t initialSegmentsNumber = 5)
    : memManager(initialSegmentSize, initialSegmentsNumber) {}

void HeapManager::printMemory() {
    std::cout << memManager.printAllSegments();
}

std::string HeapManager::getMemory() {
    return memManager.printAllSegments();
}


bool HeapManager::allocate_memory(int size) {
    std::lock_guard<std::mutex> lock(mtx);

    // Step 1: Find the best fitting segment
    Segment* bestFitSegment = nullptr;
    Segment* current = memManager.getMemoryHead();

    while (current != nullptr) {
        // We only consider free segments
        if (current->getIsFree() && current->getSize() >= size) {
            if (bestFitSegment == nullptr || current->getSize() < bestFitSegment->getSize()) {
                bestFitSegment = current;
            }
        }
        current = current->getNextMemory();
    }

    // Step 2: If no fitting segment was found, return false
    if (bestFitSegment == nullptr) {
        std::cout << "No sufficient free memory available." << std::endl;
        return false;
    }

    // Step 3: If the segment is significantly larger, split it
    if (bestFitSegment->getSize() > size) {
        return memManager.allocateSeg(bestFitSegment, size);
    }
}

bool HeapManager::free_memory(int address) {
    std::lock_guard<std::mutex> lock(mtx);
    
    Segment* current = memManager.getMemoryHead();

    while (current != nullptr) {
        if (current->getAddress() == address) {
            memManager.deallocateSeg(current);
            return true;
        }
        current = current->getNextMemory();
    }

    std::cout << "Memory at address " << address << " not found!" << std::endl;
    return false;
}