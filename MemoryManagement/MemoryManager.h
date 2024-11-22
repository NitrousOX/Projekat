#include "Segment.h"

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


public:
    MemoryManager(size_t initialSegmentSize, size_t initialSegmentsNumber = 5);
    ~MemoryManager();

    bool allocateSeg(Segment* ptr);
    bool deallocateSeg(Segment* ptr);
    bool createSeg();
    bool deleteSeg(Segment* ptr);
    void cleanupSeg();

    Segment* getMemoryHead();

};

