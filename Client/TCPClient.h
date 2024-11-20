#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> // For sockaddr_in and inet_pton

#pragma comment(lib, "Ws2_32.lib") // Link with the Winsock library

class TCPClient {
private:
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    bool isConnected;

public:
    TCPClient();
    ~TCPClient();

    void connectToServer(const std::string& ipAddress, int port);
    void sendMessage(const std::string& message);
    std::string receiveMessage();
};

#endif // TCPCLIENT_HPP
