#include "Segment.h"
#include <string>

//Treba menjati da prati dodatno i slobodnu memoriju 
//Treba napraviti i tail
#pragma once
class MemoryManager
{
private:
    size_t initialSegmentSize;
    Segment* headAllMemory;
    Segment* tailAllMemory;
    size_t totalSegments;
    size_t totalFreeSegments;

public:
    MemoryManager(size_t initialSegmentSize, size_t initialSegmentsNumber = 5);
    ~MemoryManager();

    bool allocateSeg(Segment* ptr, int size);
    bool deallocateSeg(Segment* ptr);
    bool createSeg();
    bool deleteSeg(Segment* ptr);
    void cleanupAllSegments();


    Segment* getMemoryHead();
    Segment* getMemoryTail();

    std::string printAllSegments() const;
    void cleanupSeg();
};