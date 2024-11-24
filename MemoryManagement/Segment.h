#ifndef SEGMENT_H
#define SEGMENT_H

#include <cstdlib>


class Segment {
private:
    size_t segAdress;
    Segment* nextMemory;
    Segment* previousMemory;
    size_t size;
    bool isFree = true;

public:
    // Constructor
    Segment(size_t size);

    // Destructor
    ~Segment();

    //Geter setter for next memory segment
    Segment* getNextMemory() const {
        return nextMemory;
    }

    void setNextMemory(Segment* newNext) {
        nextMemory = newNext;
    }

    Segment* getPreviousMemory() const {
        return previousMemory;
    }

    void setPreviousMemory(Segment* newPrevious) {
        previousMemory = newPrevious;
    }

    // Getter for size
    size_t getSize() const {
        return size;
    }

    // Setter for size
    void setSize(size_t newSize) {
        size = newSize;
    }

    // Getter for address
    size_t getAddress() const {
        return segAdress;
    }

    // Setter for address
    void setAddress(size_t newAddress) {
        segAdress = newAddress;
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
