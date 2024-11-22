#include "Segment.h"
#include <iostream>

// Segment constructor
Segment::Segment(size_t size) : size(size), nextMemory(nullptr), previousMemory(nullptr), nextFreeMemory(nullptr), previousFreeMemory(nullptr) {
    segAdress = 0;
    isFree = true;
}

// Segment destructor
Segment::~Segment() {
}
