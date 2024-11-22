// MemoryManagement.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Server.hpp"
#include "Segment.h"
#include "MemoryManager.h"
using namespace std;

int main()
{

    try {
        Server server(8080); // Initialize server on port 8080
        server.start();      // Start the server

        
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
