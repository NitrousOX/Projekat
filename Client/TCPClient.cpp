#include "TCPClient.h"
#include <iostream>
#include <stdexcept>

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

// Receive a message from the server
std::string TCPClient::receiveMessage() {
    if (!isConnected) {
        throw std::runtime_error("Not connected to any server.");
    }

    char buffer[512] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived == SOCKET_ERROR) {
        throw std::runtime_error("Receive failed: " + std::to_string(WSAGetLastError()));
    }

    return std::string(buffer, bytesReceived);
}