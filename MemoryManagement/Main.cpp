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
        Server server(8080);
        server.start();  
        
    }
    catch (const std::exception& e) {
       cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
