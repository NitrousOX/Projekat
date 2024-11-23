// MemoryManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Server.hpp"
#include "Segment.h"
#include "MemoryManager.h"
using namespace std;

int main()
{
    MemoryManager mm(1024,3);
    mm.printAllSegments();
    mm.allocateSeg(mm.getMemoryHead()->getNextMemory(), 658);
    mm.printAllSegments();
    mm.deallocateSeg(mm.getMemoryHead()->getNextMemory());
    mm.cleanupSeg();
    mm.printAllSegments();

    try {
        Server server(8080); // Initialize server on port 8080
        server.start();      // Start the server

        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
