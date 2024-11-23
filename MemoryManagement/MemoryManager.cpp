#include "MemoryManager.h"
#include <iostream>
#include <cstdlib>

#define MAX_FREE_SEGMENTS 5

// MemoryManager constructor
MemoryManager::MemoryManager(size_t initialSegmentSize, size_t initialSegmentsNumber)
    : initialSegmentSize(initialSegmentSize), headAllMemory(nullptr), tailAllMemory(nullptr), totalSegments(0) {

    // Allocate the initial segments
    for (size_t i = 0; i < initialSegmentsNumber; ++i) {
        createSeg();
    }
}

// MemoryManager destructor
MemoryManager::~MemoryManager() {
    // Release all segments when the manager is destroyed
    cleanupSeg();
}

//Treba menjati velicinu trenutnog segmenta i kreirati novi
// Set segment as used
bool MemoryManager::allocateSeg(Segment* ptr, int size) {
    try {
        if (size == initialSegmentSize) {
            ptr->setIsFree(false);
            return true;
        }
        if (size > initialSegmentSize)
            return false;

        // Create a new segment
        Segment* newSegment = new Segment(initialSegmentSize);

        // Update the current segment
        ptr->setIsFree(false);
        ptr->setSize(size);

        // Relocate pointers in the all memory list (next/previous memory)
        newSegment->setNextMemory(ptr->getNextMemory());
        if (ptr->getNextMemory() != nullptr) {
            ptr->getNextMemory()->setPreviousMemory(newSegment);
        }
        else {
            // If `ptr` was the tail, update `tailAllMemory`
            tailAllMemory = newSegment;
        }

        ptr->setNextMemory(newSegment);
        newSegment->setPreviousMemory(ptr);

        return true;
    }
    catch (...) {
        std::cout << "Desila se greska prilikom oslobadjanja mesta u segmentu" << std::endl;
        return false;
   }
}
// Deallocate a memory segment
bool MemoryManager::deallocateSeg(Segment* ptr) {
    try {
        ptr->setIsFree(true);
        return true;
        cleanupSeg();
    }
    catch (...) {
        std::cout << "Desila se greska prilikom zauzimanja mesta u segmentu" << std::endl;
        return false;
    }
}

//Napraviti da se adresa automatski generise
bool MemoryManager::createSeg() {

    try {
        Segment* newSegment = new Segment(initialSegmentSize);

        if (headAllMemory == nullptr && tailAllMemory == nullptr) {
            headAllMemory = newSegment;
            tailAllMemory = newSegment;

            return true;
        }

        tailAllMemory->setNextMemory(newSegment);
        newSegment->setPreviousMemory(tailAllMemory);
        size_t prevAddr = tailAllMemory->getAddress();
        newSegment->setAddress(prevAddr + tailAllMemory->getSize());
        tailAllMemory = newSegment;
        ++totalSegments;

        return true;
    }
    catch (...) {
        std::cout << "Desila se greska prilikom kreiranja novog segmenta!" << std::endl;
        return false;
    }
}


bool MemoryManager::deleteSeg(Segment* ptr) {
    if (ptr == nullptr) {
        std::cerr << "Deallocate failed: Pointer not found!\n";
        return false;
    }

    // Handle single-element list
    if (ptr == headAllMemory && ptr == tailAllMemory) {
        delete ptr;
        headAllMemory = nullptr;
        tailAllMemory = nullptr;
        --totalSegments;
        return true;
    }

    // Handle deleting head
    if (ptr == headAllMemory) {
        headAllMemory = ptr->getNextMemory();
        headAllMemory->setPreviousMemory(nullptr);
        delete ptr;
        --totalSegments;
        return true;
    }

    // Handle deleting tail
    if (ptr == tailAllMemory) {
        tailAllMemory = ptr->getPreviousMemory();
        tailAllMemory->setNextMemory(nullptr);
        delete ptr;
        --totalSegments;
        return true;
    }

    // Handle middle element
    Segment* next = ptr->getNextMemory();
    Segment* previous = ptr->getPreviousMemory();

    if (previous) {
        previous->setNextMemory(next);
    }
    if (next) {
        next->setPreviousMemory(previous);
    }

    delete ptr;
    --totalSegments;

    return true;
}


Segment* MemoryManager::getMemoryHead() {
    return headAllMemory;
}

Segment* MemoryManager::getMemoryTail() {
    return tailAllMemory;
}

void MemoryManager::printAllSegments() {
    Segment* curr = headAllMemory;
    while(curr != nullptr) {
        std::cout << "Address: " << curr->getAddress() << std::endl << "isFree: " << curr->getIsFree() << std::endl << "Size: " << curr->getSize() << std::endl;
        curr = curr->getNextMemory();
    }
}

//TODO: treba obrisati segmente ako ih ima vise od 5 slobodnih
void MemoryManager::cleanupSeg() {
    
}
