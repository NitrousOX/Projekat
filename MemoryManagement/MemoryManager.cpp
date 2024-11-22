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
bool MemoryManager::allocateSeg(Segment* ptr) {
    try {
        ptr->setIsFree(false);
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
    Segment* current = headAllMemory;
    Segment* previous = nullptr;

    // Find the segment containing the given pointer
    while (current != nullptr && current != ptr) {
        previous = current;
        current = current->getNextMemory();
    }

    if (current != nullptr) {
        // Remove the segment from the list
        if (previous != nullptr) {
            previous->setNextMemory(current->getNextMemory());
        }
        else {
            headAllMemory = current->getNextMemory();  // Removing the first element
        }

        //deleting dangling pointers
        delete current;
        delete previous;
        current = nullptr;
        previous = nullptr;
        --totalSegments;
        return true;
    }
    else {
        std::cerr << "Deallocate failed: Pointer not found!\n";
        delete current;
        delete previous;
        current = nullptr;
        previous = nullptr;
        return false;
    }
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
        std::cout << "Address: " << curr->getAddress() << std::endl << "isFree: " << curr->getIsFree() << std::endl;
        curr = curr->getNextMemory();
    }
}

//TODO: treba obrisati segmente ako ih ima vise od 5 slobodnih
void MemoryManager::cleanupSeg() {
    
}
