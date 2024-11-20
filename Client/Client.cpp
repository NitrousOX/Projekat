#include <iostream>
#include "Menu.cpp"
#include "TCPClient.h"
using namespace std;


int main()
{
    Menu();

    try {
        TCPClient client;

        // Connect to the server
        client.connectToServer("127.0.0.1", 8080);

        // Send a message to the server
        client.sendMessage("Hello, Server!");

        // Receive a message from the server
        std::string serverResponse = client.receiveMessage();
        std::cout << "Server response: " << serverResponse << "\n";

    }
    catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
    }
    return 0;
}

