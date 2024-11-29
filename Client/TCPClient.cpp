#include "TCPClient.h"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <thread>

TCPClient::TCPClient() : clientSocket(INVALID_SOCKET), isConnected(false) {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }
}

// Destructor: Cleanup resources
TCPClient::~TCPClient() {
    if (isConnected) {
        closesocket(clientSocket);
    }
    WSACleanup();
}

void TCPClient::connectToServer(const std::string& ipAddress, int port) {
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, ipAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        throw std::runtime_error("Invalid IP address.");
    }

    int result = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        closesocket(clientSocket);
        throw std::runtime_error("Connection failed: " + std::to_string(WSAGetLastError()));
    }

    isConnected = true;
    std::cout << "Connected to server: " << ipAddress << ":" << port << "\n";
}

void TCPClient::sendMessage(const std::string& message) {
    if (!isConnected) {
        throw std::runtime_error("Not connected to any server.");
    }

    int bytesSent = send(clientSocket, message.c_str(), message.size(), 0);
    if (bytesSent == SOCKET_ERROR) {
        throw std::runtime_error("Send failed: " + std::to_string(WSAGetLastError()));
    }
}

std::string TCPClient::receiveMessage() {
    if (!isConnected) {
        throw std::runtime_error("Not connected to any server.");
    }

    char buffer[2048] = { 0 };

    // Set the socket to non-blocking mode temporarily
    u_long nonBlocking = 1;
    ioctlsocket(clientSocket, FIONBIO, &nonBlocking);

    int bytesReceived = 0;
    auto startTime = std::chrono::high_resolution_clock::now();

    // Wait for the server's response, or timeout after 'timeout_ms' milliseconds
    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived == SOCKET_ERROR) {
            int errorCode = WSAGetLastError();
            if (errorCode == WSAEWOULDBLOCK) {
                // If the socket would block, check if we have reached the timeout
                auto elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
                if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsedTime).count() >= 5000) {
                    throw std::runtime_error("Receive timeout reached");
                }
                // Sleep briefly to prevent excessive CPU usage while waiting
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
            else {
                throw std::runtime_error("Receive failed: " + std::to_string(errorCode));
            }
        }
        else if (bytesReceived > 0) {
            // Data received
            break;
        }
        else {
            // Connection closed by server
            throw std::runtime_error("Connection closed by server.");
        }
    }

    // Set the socket back to blocking mode after receiving the message
    nonBlocking = 0;
    ioctlsocket(clientSocket, FIONBIO, &nonBlocking);

    return std::string(buffer, bytesReceived);
}