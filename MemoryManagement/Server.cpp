#include "Server.hpp"
#include "CircularBuffer.hpp"
#include "HeapManager.hpp"
#include <thread>
#include <vector>
#include <mutex>
#include <array>
#include "ThreadMonitor.h" 

#define BUFFER_SIZE 1024

using namespace std;

Server::Server(int port) : port(port), serverSocket(INVALID_SOCKET), stopFlag(false) {
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

void Server::worker(CircularBuffer& cb, HeapManager& hm) {
    while (!stopFlag.load()) {
        ExtendedClientRequest request;

        if (cb.remove(request)) {
            monitor.threadStarted();  // Notify monitor that a worker thread has started
            string response;

            switch (request.getRequestType()) {
            case 1:
                cout << "Allocating memory..." << endl;
                hm.allocate_memory(request.getData());
                break;
            case 2:
                cout << "Deallocating memory..." << endl;
                hm.free_memory(request.getData());
                break;
            case 3:
                cout << "Printing memory to client..." << endl;
                response = hm.getMemory();
                send(request.getClientSocket(), response.c_str(), response.length(), 0);
                break;
            default:
                cout << "Wrong request!" << endl;
                break;
            }
            hm.printMemory();
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
    HeapManager heapMngr(1024, 5);

    // Launch worker threads
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(&Server::worker, this, ref(cb), ref(heapMngr));
    }

    cout << "Server listening on port " << port << endl;

    while (!stopFlag.load()) {
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
    stopFlag.store(true);

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

    while (!stopFlag.load()) {
        int bytesRead = recv(clientSocket, buffer.data(), BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            cout << "Client disconnected." << endl;
            closesocket(clientSocket);
            break;
        }

        buffer[bytesRead] = '\0';  // Null-terminate the string


        ClientRequest request = ClientRequest::deserialize(std::string(buffer.data(), bytesRead));
        ExtendedClientRequest exReq = ExtendedClientRequest(request.getData(), request.getRequestType(), clientSocket);

        cb.add(exReq);
        monitor.threadStarted();
        std::string response = "Request received: " + exReq.getRequestType();
        send(clientSocket, response.c_str(), response.length(), 0);
    }
    monitor.threadFinished();
}

Server::~Server() {
    stop();
}