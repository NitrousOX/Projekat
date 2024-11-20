#ifndef SEGMENT_H
#define SEGMENT_H

#include <cstdlib>  

class Segment {
public:
    void* data;
    Segment* next;

    Segment(size_t size);

    ~Segment();
};

#endif // SEGMENT_H
