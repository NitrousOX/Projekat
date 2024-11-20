#include "Segment.h"
#include <iostream>

// Segment constructor
Segment::Segment(size_t size) : size(size), next(nullptr) {
    data = malloc(size);  // Allocate memory of the specified size
    if (!data) {
        std::cerr << "Memory allocation failed!\n";
        exit(EXIT_FAILURE);
    }
    Segment::isFree = true;
}

// Segment destructor
Segment::~Segment() {
    free(data);  // Free the allocated memory
}
