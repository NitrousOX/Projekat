#include "MemoryManager.h"
#include <iostream>
#include <cstdlib>

#define MAX_FREE_SEGMENTS 5

// MemoryManager constructor
MemoryManager::MemoryManager(size_t segmentSize, size_t initialSegments)
    : segmentSize(segmentSize), head(nullptr), totalSegments(0) {

    // Allocate the initial segments
    for (size_t i = 0; i < initialSegments; ++i) {
        allocate();
    }
}

// MemoryManager destructor
MemoryManager::~MemoryManager() {
    // Release all segments when the manager is destroyed
    cleanup();
}

// Allocate a new memory segment and add it to the list
void* MemoryManager::allocate() {
    // Create a new segment
    Segment* newSegment = new Segment(segmentSize);

    // Use setter to add the new segment to the front of the list
    newSegment->setNext(head);  // Add the new segment to the front of the list
    head = newSegment;
    ++totalSegments;

    // Set the segment as not free
    newSegment->setIsFree(false);

    return newSegment->getData();  // Return the allocated memory block
}

// Deallocate a memory segment
void MemoryManager::deallocate(void* ptr) {
    Segment* current = head;
    Segment* previous = nullptr;

    // Find the segment containing the given pointer
    while (current != nullptr && current->getData() != ptr) {
        previous = current;
        current = current->getNext();
    }

    if (current != nullptr) {
        // Remove the segment from the list
        if (previous != nullptr) {
            previous->setNext(current->getNext());
        }
        else {
            head = current->getNext();  // Removing the first element
        }

        delete current;  // Free the memory of this segment
        --totalSegments;
    }
    else {
        std::cerr << "Deallocate failed: Pointer not found!\n";
    }

    // Cleanup if there are more than 5 segments
    cleanup();
}

// Cleanup unused segments if there are more than 5
void MemoryManager::cleanup() {
    // If there are more than 5 segments, release unused segments
    if (totalSegments > MAX_FREE_SEGMENTS) {
        Segment* current = head;
        Segment* previous = nullptr;
        size_t count = 0;

        // Traverse the list and release segments
        while (current != nullptr && count < totalSegments - MAX_FREE_SEGMENTS) {
            previous = current;
            current = current->getNext();

            // Only delete previous after it has been updated
            delete previous;  // Free the segment
            --totalSegments;
            ++count;
        }

        // If there are remaining segments, ensure the last one points to nullptr
        if (current != nullptr) {
            head = current;
            previous->setNext(nullptr);  // previous is now the last segment
        }
    }
}
