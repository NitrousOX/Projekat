#include <iostream>
#include "TCPClient.h"
#include "../MemoryManagement/ClientRequest.h"
using namespace std;

int static Menu(TCPClient& client) {
    int choice;
    int size = 0;
    int address = 0;
    string serializedRequest;
    ClientRequest cr;  // Declare 'cr' before the switch statement

    do {
        cout << "\n";
        cout << "=============================\n";
        cout << "         MAIN MENU           \n";
        cout << "=============================\n";
        cout << "  1. Allocate Memory         \n";
        cout << "  2. Deallocate Memory       \n";
        cout << "  3. Exit                    \n";
        cout << "=============================\n";
        cout << "Please enter your choice: ";
        cin >> choice;

        // Handle user choice
        switch (choice) {
        case 1:
            cout << "\nWhat is the size you want to allocate:\n";
            cin >> size;
            cr = ClientRequest(size, choice);  // Initialize 'cr' here
            serializedRequest = cr.serialize();
            cout << "\nYou selected: Allocate Memory\n";
            client.sendMessage(serializedRequest);  // Send message to server
            break;
        case 2:
            cout << "\nInsert the address you want to deallocate:\n";
            cin >> address;
            cr = ClientRequest(address, choice);
            serializedRequest = cr.serialize();
            client.sendMessage(serializedRequest);  // Send message to server
            break;
        case 3:
            cout << "\nThank you for using the program. Goodbye!\n";
            break;
        default:
            cout << "\nInvalid choice. Please try again.\n";
        }

        // Pause to show the result before clearing the screen
        if (choice != 3) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get(); // Wait for Enter key
        }

    } while (choice != 3);

    return 0;
}
