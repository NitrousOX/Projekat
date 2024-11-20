#include <iostream>
#include "Menu.cpp"
#include "TCPClient.h"
using namespace std;


int main() {
    try {
        TCPClient client;
        client.connectToServer("192.168.0.134", 8080); // Connect to the server (use the correct IP and port)

        Menu(client); // Call the menu function and pass the TCPClient instance

    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}