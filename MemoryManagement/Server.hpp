#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <winsock2.h> // Windows Sockets API
#include <ws2tcpip.h> // For sockaddr_in
#include "CircularBuffer.hpp"

#pragma comment(lib, "Ws2_32.lib") // Link with Winsock library

class Server {
private:
    //TODO:Circular buffer
   // CircularBuffer<std::string> requestBuffer; // Buffer to store client requests
    SOCKET serverSocket;             // Server socket file descriptor
    int port;                        // Port number to bind the server
    sockaddr_in serverAddr;          // Server address structure
    std::vector<std::thread> threads; // Vector to store client threads
    const int BUFFER_SIZE = 1024;    // Size of the buffer for client messages

    // Handles communication with a single client
    void handleClient(SOCKET clientSocket);

public:

    Server(int port);

    void start();

    void stop();

    ~Server();
};


#endif // !SERVER_H
