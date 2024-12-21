#include <iostream>
#include "Menu.cpp"
#include "TCPClient.h"
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

void ClientApp();
void StressTest();
void HighFrequencyStressTest();

int main() {
    int chosenType;
    do {
        printf("Stress test (1) | Client app(2) | High-Frequency Stress Test (3)\n 4->exit");

        std::cin >> chosenType;
        switch (chosenType) {
        case 1:
            StressTest();
            break;
        case 2:
            ClientApp();
            break;
        case 3:
            HighFrequencyStressTest();
            break;
        default:
            cout << "Invalid choice. Please select 1, 2, or 3.\n";
            break;
        }
    } while (chosenType != 4);

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

void HighFrequencyStressTest() {
    cout << "Starting High-Frequency Stress Test...\n";

    const int connectionCount = 20; // Number of connections to simulate
    const int messageCount = 1000; // Number of messages each client sends
    vector<thread> threads;
    mutex outputLock;

    for (int i = 0; i < connectionCount; ++i) {
        threads.emplace_back([i, &outputLock, messageCount]() {
            try {
                TCPClient client;
                client.connectToServer("127.0.0.1", 8080);

                {
                    lock_guard<mutex> lock(outputLock);
                    cout << "Connection " << i + 1 << " established.\n";
                }

                // Send multiple messages rapidly
                for (int j = 0; j < messageCount; ++j) {
                    ClientRequest cr = ClientRequest(123, 1);
                    string serializedRequest = cr.serialize();
                    client.sendMessage(serializedRequest);
                }

                {
                    lock_guard<mutex> lock(outputLock);
                    cout << "Connection " << i + 1 << " completed sending " << messageCount << " messages.\n";
                }
            }
            catch (const exception& e) {
                lock_guard<mutex> lock(outputLock);
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

    cout << "High-Frequency Stress Test Completed.\n";
}


void StressTest() {
    cout << "Starting Connection Stress Test...\n";

    // Simulate multiple TCPClient connections
    int connectionCount = 20; // Number of connections to simulate
    vector<thread> threads;
    mutex outputLock;

    cout << "Enter number of connections: " << endl;
    cin >> connectionCount;

    for (int i = 0; i < connectionCount; ++i) {
        threads.emplace_back([i, &outputLock]() {
            try {
                TCPClient client;
                client.connectToServer("127.0.0.1", 8080); // Adjust server IP/port
                {
                    lock_guard<mutex> lock(outputLock);
                    cout << "Connection " << i + 1 << " successful.\n";
                }
                //Allocate memory for 128 bytes
                ClientRequest cr = ClientRequest(123, 1);
                string serializedRequest = cr.serialize();
                client.sendMessage(serializedRequest);

                cr = ClientRequest(0, 4);
                serializedRequest = cr.serialize();
                client.sendMessage(serializedRequest);
            }
            catch (const exception& e) {
                lock_guard<mutex> lock(outputLock);
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