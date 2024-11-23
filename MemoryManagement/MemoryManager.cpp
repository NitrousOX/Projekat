#include "MemoryManager.h"
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>

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
        newSegment->setSize(initialSegmentSize - size);
        newSegment->setAddress(ptr->getAddress() + size);

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
        return true;
        cleanupSeg();
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

void MemoryManager::printAllSegments() const {
    const Segment* current = headAllMemory;

    std::cout << "\nMemory Segments Visualization:\n";
    std::cout << "------------------------------------------------------------\n";

    while (current != nullptr) {
        // Build the content dynamically
        std::string id = "Address: " + std::to_string(current->getAddress());
        std::string size = "Size: " + std::to_string(current->getSize());
        std::string free = "Free: " + std::string(current->getIsFree() ? "Yes" : "No");

        // Determine box width dynamically based on the longest content
        size_t boxWidth = std::max({ id.length(), size.length(), free.length() }) + 4;

        // Choose color for borders and content based on isFree
        std::string borderColor = current->getIsFree() ? "\033[1;32m" : "\033[1;31m"; // Green for free, Red for allocated
        std::string textColor = "\033[0m"; // Default text color (white/neutral)

        // Construct the box with colored borders
        std::string topBottomBorder = borderColor + "+" + std::string(boxWidth, '-') + "+" + textColor;
        std::string idLine = borderColor + "| " + textColor + id + std::string(boxWidth - id.length() - 2, ' ') + borderColor + " |" + textColor;
        std::string sizeLine = borderColor + "| " + textColor + size + std::string(boxWidth - size.length() - 2, ' ') + borderColor + " |" + textColor;
        std::string freeLine = borderColor + "| " + textColor + free + std::string(boxWidth - free.length() - 2, ' ') + borderColor + " |" + textColor;

        // Print the segment
        std::cout << topBottomBorder << "\n";
        std::cout << idLine << "\n";
        std::cout << sizeLine << "\n";
        std::cout << freeLine << "\n";
        std::cout << topBottomBorder << "\n";

        // Move to the next segment
        current = current->getNextMemory();
    }

    std::cout << "------------------------------------------------------------\n";
}

void MemoryManager::cleanupSeg() {
    // If no memory segments exist, do nothing
    if (!headAllMemory) {
        std::cout << "No memory segments to clean up." << std::endl;
        return;
    }

    Segment* current = headAllMemory;
    Segment* smallestFree = nullptr;

    // Loop to find the smallest free segment
    while (current) {
        if (current->getIsFree()) {
            if (!smallestFree || current->getSize() < smallestFree->getSize()) {
                smallestFree = current;
            }
        }
        current = current->getNextMemory();
    }

    // If no free segment found, nothing to clean
    if (!smallestFree) {
        std::cout << "No free segments to clean up." << std::endl;
        return;
    }

    // Remove the smallest free segment
    std::cout << "Cleaning up segment at address: " << smallestFree->getAddress()
        << " of size: " << smallestFree->getSize() << std::endl;

    // Adjust pointers around the segment
    if (smallestFree->getPreviousMemory()) {
        smallestFree->getPreviousMemory()->setNextMemory(smallestFree->getNextMemory());
    }
    else {
        headAllMemory = smallestFree->getNextMemory();
    }

    if (smallestFree->getNextMemory()) {
        smallestFree->getNextMemory()->setPreviousMemory(smallestFree->getPreviousMemory());
    }
    else {
        tailAllMemory = smallestFree->getPreviousMemory();
    }

    // Free the memory occupied by the segment
    delete smallestFree;
    totalSegments--;
}
