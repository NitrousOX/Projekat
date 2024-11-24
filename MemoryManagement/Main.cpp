#include <iostream>
#include "Server.hpp"
#include "Segment.h"
#include "MemoryManager.h"
using namespace std;

int main()
{

    try {
        Server server(8080);
        server.start();  
        
    }
    catch (const std::exception& e) {
       cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
