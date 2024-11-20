#include "Segment.h"

#pragma once
class MemoryManager
{
public:
    MemoryManager(size_t segmentSize, size_t initialSegments = 5);
    ~MemoryManager();

    void* allocate();
    void deallocate(void* ptr);
    void cleanup();  // Release unused segments if there are more than 5

private:
    size_t segmentSize;   // The size of each memory segment
    Segment* head;        // Head of the segment list
    size_t totalSegments; // The current number of segments
};

