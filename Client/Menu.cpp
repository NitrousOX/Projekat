#include <iostream>
#include <cstdlib> // For system commands
using namespace std;

int static Menu() {
    int choice;

    do {
        // Display menu
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
            cout << "\nYou selected: Allocate Memory\n";
            break;
        case 2:
            cout << "\nYou selected: Deallocate Memory\n";
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
