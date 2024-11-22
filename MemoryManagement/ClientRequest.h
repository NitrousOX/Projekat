#pragma once
#include <string>
#include <cstring> // For memcpy

class ClientRequest
{
private:
    int data;           // Represents the data or size of the request
    int requestType;    // Represents the type of request (e.g., 1 for allocate, 2 for deallocate)

public:
    // Default constructor
    ClientRequest() : data(0), requestType(0) {}

    // Parameterized constructor
    ClientRequest(int newData, int newType) : data(newData), requestType(newType) {}

    // Getter for 'data'
    int getData() const {
        return data;
    }

    // Setter for 'data'
    void setData(int newData) {
        data = newData;
    }

    // Getter for 'requestType'
    int getRequestType() const {
        return requestType;
    }

    // Setter for 'requestType'
    void setRequestType(int newRequestType) {
        requestType = newRequestType;
    }

    // Serialization: Convert the object into a string or byte array
    std::string serialize() const {
        std::string serialized;

        // Convert 'data' to bytes and append to the string
        serialized.append(reinterpret_cast<const char*>(&data), sizeof(data));

        // Convert 'requestType' to bytes and append to the string
        serialized.append(reinterpret_cast<const char*>(&requestType), sizeof(requestType));

        return serialized;
    }

    // Deserialization: Convert a string or byte array back to a ClientRequest object
    static ClientRequest deserialize(const std::string& data) {
        int size = 0;
        int requestType = 0;

        // Extract the 'data' from the serialized string
        std::memcpy(&size, data.data(), sizeof(size));

        // Extract the 'requestType' from the serialized string
        std::memcpy(&requestType, data.data() + sizeof(size), sizeof(requestType));

        return ClientRequest(size, requestType);
    }
};
