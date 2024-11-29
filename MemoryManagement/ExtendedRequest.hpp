#pragma once
#include "ClientRequest.h"
#include <winsock2.h> // For SOCKET

class ExtendedClientRequest : public ClientRequest
{
private:
    SOCKET clientSocket; // Represents the client socket associated with the request

public:
    // Default constructor
    ExtendedClientRequest() : ClientRequest(), clientSocket(INVALID_SOCKET) {}

    // Parameterized constructor
    ExtendedClientRequest(int newData, int newType, SOCKET newSocket)
        : ClientRequest(newData, newType), clientSocket(newSocket) {}

    // Getter for 'clientSocket'
    SOCKET getClientSocket() const {
        return clientSocket;
    }

    // Setter for 'clientSocket'
    void setClientSocket(SOCKET newSocket) {
        clientSocket = newSocket;
    }

    // Overridden serialization: Include clientSocket
    std::string serialize() const {
        std::string serialized = ClientRequest::serialize();

        // Convert 'clientSocket' to bytes and append to the string
        serialized.append(reinterpret_cast<const char*>(&clientSocket), sizeof(clientSocket));

        return serialized;
    }

    // Overridden deserialization: Include clientSocket
    static ExtendedClientRequest deserialize(const std::string& data) {
        // Deserialize base class data
        ClientRequest baseRequest = ClientRequest::deserialize(data);

        SOCKET clientSocket = INVALID_SOCKET;
        // Extract 'clientSocket' from the serialized string
        std::memcpy(&clientSocket, data.data() + sizeof(int) * 2, sizeof(clientSocket));

        return ExtendedClientRequest(baseRequest.getData(), baseRequest.getRequestType(), clientSocket);
    }
};
