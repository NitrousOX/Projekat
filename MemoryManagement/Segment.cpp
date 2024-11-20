#include "Segment.h"
#include <iostream>

// Segment constructor
Segment::Segment(size_t size) : data(nullptr), next(nullptr) {
    data = malloc(size);
    if (!data) {
        std::cerr << "Memory allocation failed!\n";
        exit(EXIT_FAILURE);
    }
}

// Segment destructor
Segment::~Segment() {
    free(data);
}
