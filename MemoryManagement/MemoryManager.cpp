#include "MemoryManager.h"
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <sstream>


#define MAX_FREE_SEGMENTS 5

// MemoryManager constructor
MemoryManager::MemoryManager(size_t initialSegmentSize, size_t initialSegmentsNumber)
    : initialSegmentSize(initialSegmentSize), headAllMemory(nullptr), tailAllMemory(nullptr), totalSegments(0), totalFreeSegments(0){

    for (size_t i = 0; i < initialSegmentsNumber; ++i) {
        createSeg();
    }
}

// MemoryManager destructor
MemoryManager::~MemoryManager() {
}

bool MemoryManager::allocateSeg(Segment* ptr, int size) {
    try {
        if (size == initialSegmentSize) {
            ptr->setIsFree(false);
            totalFreeSegments--;
            return true;
        }
        if (size > initialSegmentSize)
            return false;

        // Create a new segment
        Segment* newSegment = new Segment(initialSegmentSize);
        totalSegments++;
       

        //------------------------------------------------------------
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
        newSegment->setSize(ptr->getSize() - size);
        newSegment->setAddress(ptr->getAddress() + size);

        // Update the current segment
        ptr->setIsFree(false);
        ptr->setSize(size);

        return true;
    }
    catch (...) {
        std::cout << "Desila se greska prilikom zauzimanja mesta u segmentu" << std::endl;
        return false;
    }
}
// Deallocate a memory segment
bool MemoryManager::deallocateSeg(Segment* ptr) {
    try {
        ptr->setIsFree(true);
        totalFreeSegments++;
        while (totalFreeSegments > 5)
            cleanupSeg();
        return true;
    }
    catch (...) {
        std::cout << "Desila se greska prilikom oslobadjanja mesta u segmentu" << std::endl;
        return false;
    }
}

//Napraviti da se adresa automatski generise
bool MemoryManager::createSeg() {

    try {
        Segment* newSegment = new Segment(initialSegmentSize);
        ++totalFreeSegments;
        ++totalSegments;

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

    if (ptr->getIsFree())
        totalFreeSegments--;

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


std::string MemoryManager::printAllSegments() const {
    const Segment* current = headAllMemory;
    std::ostringstream output;

    output << "\nMemory Segments Visualization:\n";
    output << "------------------------------------------------------------\n";

    while (current != nullptr) {
        // Build the content dynamically
        std::string id = "Address: " + std::to_string(current->getAddress());
        std::string size = "Size: " + std::to_string(current->getSize());
        std::string free = "Free: " + std::string(current->getIsFree() ? "Yes" : "No");

        // Determine box width dynamically based on the longest content
        size_t boxWidth = std::max({ id.length(), size.length(), free.length() }) + 4;

        // Construct the box
        std::string topBottomBorder = "+" + std::string(boxWidth, '-') + "+";
        std::string idLine = "| " + id + std::string(boxWidth - id.length() - 2, ' ') + " |";
        std::string sizeLine = "| " + size + std::string(boxWidth - size.length() - 2, ' ') + " |";
        std::string freeLine = "| " + free + std::string(boxWidth - free.length() - 2, ' ') + " |";

        // Append the segment to the output
        output << topBottomBorder << "\n";
        output << idLine << "\n";
        output << sizeLine << "\n";
        output << freeLine << "\n";
        output << topBottomBorder << "\n";

        // Move to the next segment
        current = current->getNextMemory();
    }

    output << "------------------------------------------------------------\n";
    return output.str();
}


void MemoryManager::cleanupSeg() {
    if (!headAllMemory) {
        std::cout << "No memory segments to clean up." << std::endl;
        return;
    }

    Segment* current = headAllMemory;
    Segment* smallestFree = nullptr;

    while (current) {
        if (current->getIsFree()) {
            if (!smallestFree || current->getSize() < smallestFree->getSize()) {
                smallestFree = current;
            }
        }
        current = current->getNextMemory();
    }

    if (!smallestFree) {
        std::cout << "No free segments to clean up." << std::endl;
        return;
    }

    std::cout << "Cleaning up segment at address: " << smallestFree->getAddress()
        << " of size: " << smallestFree->getSize() << std::endl;

    deleteSeg(smallestFree);
}
