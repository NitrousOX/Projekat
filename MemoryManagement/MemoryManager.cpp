#include "MemoryManager.h"
#include <iostream>
#include <cstdlib>

#define MAX_FREE_SEGMENTS 5

// MemoryManager constructor
MemoryManager::MemoryManager(size_t initialSegmentSize, size_t initialSegmentsNumber)
    : initialSegmentSize(initialSegmentSize), headAllMemory(nullptr), totalSegments(0) {

    // Allocate the initial segments
    for (size_t i = 0; i < initialSegmentsNumber; ++i) {
        createSeg();
    }
}

// MemoryManager destructor
MemoryManager::~MemoryManager() {
    // Release all segments when the manager is destroyed
    cleanupSeg();
}


//NOTE:Mislim da bi trebalo napraviti posebno create i allocate metode create bi napravilo nove segmente/segment i sve bi bilo isFree = ture
//dok allocate bi dobio broj/velicinu segmenta koji treba da zauzme koje je create() napravio
//Znaci inicijalno kad se pokrene program create() se pozove da kreira sve segmente i sa njim bi se dodali novi segmenti
//a allocate se poziva kad client kaze koliko mesta zeli pa sa bestfit algoritmom allociramo toliko segmenata da ispuni zahtev;
//Tu u memory manageru se cuva head liste segmenata i on pretrazuje/dodaje/brise elem


//Treba menjati velicinu trenutnog segmenta i kreirati novi
// Set segment as used
bool MemoryManager::allocateSeg(Segment* ptr) {
    try {
        ptr->setIsFree(false);
        return true;
    }
    catch (...) {
        std::cout << "Desila se greska prilikom oslobadjanja mesta u segmentu" << std::endl;
        return false;
   }
}
// Deallocate a memory segment
bool MemoryManager::deallocateSeg(Segment* ptr) {
    try {
        ptr->setIsFree(true);
        return true;
        cleanupSeg();
    }
    catch (...) {
        std::cout << "Desila se greska prilikom zauzimanja mesta u segmentu" << std::endl;
        return false;
    }
}

//Napraviti da se adresa automatski generise
bool MemoryManager::createSeg() {

    try {
        Segment* newSegment = new Segment(initialSegmentSize);

        newSegment->setNextMemory(headAllMemory);
        headAllMemory = newSegment;
        ++totalSegments;

        // Segment je slobodan kad se kreira
        newSegment->setIsFree(true);


        delete newSegment;
        newSegment = nullptr;
        return true;
    }
    catch (...) {
        std::cout << "Desila se greska prilikom kreiranja novog segmenta!" << std::endl;
        return false;
    }
}


bool MemoryManager::deleteSeg(Segment* ptr) {
    Segment* current = headAllMemory;
    Segment* previous = nullptr;

    // Find the segment containing the given pointer
    while (current != nullptr && current != ptr) {
        previous = current;
        current = current->getNextMemory();
    }

    if (current != nullptr) {
        // Remove the segment from the list
        if (previous != nullptr) {
            previous->setNextMemory(current->getNextMemory());
        }
        else {
            headAllMemory = current->getNextMemory();  // Removing the first element
        }

        //deleting dangling pointers
        delete current;
        delete previous;
        current = nullptr;
        previous = nullptr;
        --totalSegments;
        return true;
    }
    else {
        std::cerr << "Deallocate failed: Pointer not found!\n";
        delete current;
        delete previous;
        current = nullptr;
        previous = nullptr;
        return false;
    }
}

Segment* MemoryManager::getMemoryHead() {
    return headAllMemory;
}


//TODO: treba obrisati segmente ako ih ima vise od 5 slobodnih
void MemoryManager::cleanupSeg() {
    
}
