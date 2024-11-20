#ifndef SEGMENT_H
#define SEGMENT_H

#include <cstdlib>

class Segment {
private:
    void* data;         // Pointer to the allocated memory block
    Segment* next;      // Pointer to the next segment in the list
    size_t size;        // Size of the segment's memory block
    bool isFree;

public:
    // Constructor
    Segment(size_t size);

    // Destructor
    ~Segment();

    // Getter for data
    void* getData() const {
        return data;
    }

    // Setter for data
    void setData(void* newData) {
        data = newData;
    }

    // Getter for next segment
    Segment* getNext() const {
        return next;
    }

    // Setter for next segment
    void setNext(Segment* newNext) {
        next = newNext;
    }

    // Getter for size
    size_t getSize() const {
        return size;
    }

    // Setter for size
    void setSize(size_t newSize) {
        size = newSize;
    }

    // Getter for isFree
    bool getIsFree() const {
        return isFree;
    }

    // Setter for isFree
    void setIsFree(bool newIsFree) {
        isFree = newIsFree;
    }
};

#endif // SEGMENT_H
