#include "Server.hpp"
#include "CircularBuffer.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <array>
#include "ThreadMonitor.h" 

#define BUFFER_SIZE 1024

using namespace std;

Server::Server(int port) : port(port), serverSocket(INVALID_SOCKET) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw runtime_error("WSAStartup failed");
    }

    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        WSACleanup();
        throw runtime_error("Socket creation failed");
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);
}

void Server::worker(CircularBuffer& cb) {
    while (true) {
        ClientRequest request;

        if (cb.remove(request)) {
            monitor.threadStarted();  // Notify monitor that a worker thread has started
            cout << "Processing request: " << request.getRequestType() << endl;
            monitor.threadFinished();  // Notify monitor that the worker thread has finished
        }
    }
}

void Server::start() {
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        stop();
        throw runtime_error("Bind failed");
    }

    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        stop();
        throw runtime_error("Listen failed");
    }

    CircularBuffer cb;

    // Launch worker threads
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(&Server::worker, this, ref(cb));
    }

    cout << "Server listening on port " << port << endl;

    while (true) {
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
            continue;
        }

        cout << "New client connected." << endl;

        // Launch a new thread to handle the client
        threads.emplace_back(&Server::handleClient, this, clientSocket, ref(cb));
    }
}

void Server::stop() {
    if (serverSocket != INVALID_SOCKET) {
        closesocket(serverSocket);
        serverSocket = INVALID_SOCKET;
    }

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

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

        buffer[bytesRead] = '\0';  // Null-terminate the string

        // Deserialize the client request
        ClientRequest request = ClientRequest::deserialize(std::string(buffer.data(), bytesRead));

        cb.add(request);
        cb.printBuffer();

        // Respond back to the client
        std::string response = "Request received: " + request.getRequestType();
        send(clientSocket, response.c_str(), response.length(), 0);
    }
}

Server::~Server() {
    stop();
}
