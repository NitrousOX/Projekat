#ifndef HEAP_MANAGER_HPP
#define HEAP_MANAGER_HPP

#include "MemoryManager.h"

#pragma once


class HeapManager {
private:
	MemoryManager memManager;
	Segment* headFreeMemory;
	Segment* tailFreeMemory;
	unsigned int freeSegments;

public:
	HeapManager(size_t initialSegmentSize, size_t initialSegmentsNumber);
	void* allocate_memory(int size);
	void free_memory(void* address);
};

#endif // !HEAP_MANAGER_HPP
