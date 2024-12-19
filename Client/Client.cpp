#include <iostream>
#include "Menu.cpp"
#include "TCPClient.h"
#include <vector>
#include <thread>
using namespace std;

void ClientApp();
void StressTest();

int main() {
    int chosenType;
    printf("Stress test (1) | Client app(2)");
    do {

        std::cin >> chosenType;
        switch (chosenType) {
            case 1:
                StressTest();
                break;
            case 2:
                ClientApp();
                break;
            default:
                cout << "Invalid choice. Please select 1 or 2.\n";
                break;
        }
    } while (!(chosenType == 1 || chosenType == 2));

   

    return 0;
}


void ClientApp() {
    try {
        TCPClient client;
        client.connectToServer("127.0.0.1", 8080); // Connect to the server (use the correct IP and port)

        Menu(client); // Call the menu function and pass the TCPClient instance

    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

void StressTest() {
    cout << "Starting Connection Stress Test...\n";

    // Simulate multiple TCPClient connections
    const int connectionCount = 20; // Number of connections to simulate
    vector<thread> threads;

    for (int i = 0; i < connectionCount; ++i) {
        threads.emplace_back([i]() {
            try {
                TCPClient client;
                client.connectToServer("127.0.0.1", 8080); // Adjust server IP/port
                cout << "Connection " << i + 1 << " successful.\n";

                //Allocate memory for 128 bytes
                ClientRequest cr = ClientRequest(123, 1);
                string serializedRequest = cr.serialize();
                client.sendMessage(serializedRequest);

                cr = ClientRequest(0, 4);
                serializedRequest = cr.serialize();
                client.sendMessage(serializedRequest);
            }
            catch (const exception& e) {
                cerr << "Connection " << i + 1 << " failed: " << e.what() << endl;
            }
            });
    }

    // Wait for all threads to complete
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    cout << "Stress Test Completed.\n";
}