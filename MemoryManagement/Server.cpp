#include "Server.hpp"
#include "CircularBuffer.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <array>


#define BUFFER_SIZE 1024

using namespace std;

//Implementirati circular buffer
Server::Server(int port) : port(port), serverSocket(INVALID_SOCKET) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw runtime_error("WSAStartup failed");
    }

    // Create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        throw runtime_error("Socket creation failed");
    }

    // Set up the server address structure
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
}

void Server::worker(CircularBuffer& cb) {
    while (true) {
        ClientRequest request;

        // Wait for a request to be available
        if (cb.remove(request)) {
            // Process the request (this could be anything based on your business logic)
            cout << "Processing request: " << request.getRequestType() << endl;
        }
    }
}


void Server::start() {
    // Bind the socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        stop();
        throw runtime_error("Bind failed");
    }

    // Listen for incoming connections
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        stop();
        throw runtime_error("Listen failed");
    }

    CircularBuffer cb;

    // Start worker threads
    for (int i = 0; i < 4; ++i) {  // Assuming 4 workers
        threads.emplace_back(&Server::worker, this, ref(cb));
    }

    cout << "Server listening on port " << port << endl;

    while (true) {
        // Accept a new client connection
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            continue;
        }

        cout << "New client connected." << endl;

        // Handle the client in a separate thread
        threads.emplace_back(&Server::handleClient, this, clientSocket, ref(cb));
    }
}


void Server::stop() {
    // Close the server socket
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    // Join all client threads
    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Clean up Winsock
    WSACleanup();
    cout << "Server stopped" << endl;
}

void Server::handleClient(SOCKET clientSocket, CircularBuffer& cb) {
    vector<char> buffer(BUFFER_SIZE);

    while (true) {
        int bytesRead = recv(clientSocket, buffer.data(), BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            cout << "Client disconnected." << endl;
            closesocket(clientSocket);
            break;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the buffer

        // Deserialize the received data into a ClientRequest
        ClientRequest request = ClientRequest::deserialize(std::string(buffer.data(), bytesRead));

        // Add the request to the circular buffer for processing by workers
        cb.add(request);
        cb.printBuffer();

        // Send an acknowledgment back to the client (optional)
        std::string response = "Request received: " + request.getRequestType();
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}



Server::~Server() {
    stop(); // Ensure the server stops and cleans up resources
}
