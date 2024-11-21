#ifndef SEGMENT_H
#define SEGMENT_H

#include <cstdlib>


class Segment {
private:
    size_t segAdress;
    Segment* nextMemory;
    Segment* nextFreeMemory;
    Segment* previousFreeMemory;
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
    //getter setter for next free memory segment
    Segment* getNextFreeMemory() const {
        if (isFree)
            return nextFreeMemory;
        else
            return nullptr;
    }

    void setNextFreeMemory(Segment* newNext) {
        nextFreeMemory = newNext;
    }
    //getter setter for previous memory segment
    Segment* getPreviousFreeMemory() const {
        if (isFree)
            return previousFreeMemory;
        else
            return nullptr;
    }

    void setPreviousFreeMemory(Segment* newPrevious) {
        previousFreeMemory = newPrevious;
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
