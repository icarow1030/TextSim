#include <httplib.h>
#include "config/config.hpp"
#include "client/client.hpp"
#include "server/AppServer.hpp"
#include "cryptography/rsa.hpp"
#include "services/services.hpp"

#include <iostream>
#include <string>
#include <cstdlib> // For system()

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

using namespace httplib;
using namespace Config;

// Function to clear the terminal screen
void clearTerminal() {
    std::system(CLEAR_COMMAND);
}

bool isServerOnline(AppServer& server) {
    return server.status() == Config::ServerStatus::ONLINE;
}


// Show main menu
void showMenu(AppServer& server) {
    std::cout << "\n==== MAIN MENU ====" << std::endl;
    std::cout << "[1] " << (isServerOnline(server) ? "Disconnect from Target Port" : "Connect to Target Port") << std::endl;
    std::cout << "[2] " << (isServerOnline(server) ? "Stop Server Mode" : "Start Server Mode") << std::endl;
    std::cout << "[3] Start Client Mode (Chat)" << std::endl;
    std::cout << "[4] Connection Settings" << std::endl;
    std::cout << "[5] User Settings" << std::endl;
    std::cout << "[0] Exit" << std::endl;
}

// Show connection settings submenu
void connectionSettingsMenu() {
    std::cout << "\n-- Connection Settings --" << std::endl;
    std::cout << "[1] Change Destination Port" << std::endl;
    std::cout << "[2] Change Current Port" << std::endl;
    std::cout << "[3] Check Connection" << std::endl;
    std::cout << "[0] Back" << std::endl;
}

// Show user settings submenu
void userSettingsMenu() {
    std::cout << "\n-- User Settings --" << std::endl;
    std::cout << "[1] Edit Username" << std::endl;
    std::cout << "[2] Generate RSA Keys" << std::endl;
    std::cout << "[0] Back" << std::endl;
}

int main() {
    // Server instance
    AppServer server;

    int option = -1;
    while (option != 0) {
        clearTerminal(); // Clear terminal at each menu display
        showMenu(server);
        std::cout << "Choose an option: ";
        std::cin >> option;
        switch (option) {
            case 1:
                if (isServerOnline(server)) {
                    std::cout << "Disconnecting..." << std::endl;
                } else {
                    std::cout << "Connecting..." << std::endl;
                }
                break;
            case 2: 
                if(isServerOnline(server)) {
                    std::cout << "Stopping server mode..." << std::endl;
                    Services::stopServerMode(server);
                } else {
                    std::cout << "Starting server mode..." << std::endl;
                    Services::startServerMode(server);
                }
                break;
            case 3:
                std::cout << "Starting client mode (chat)..." << std::endl;
                break;
            case 4: {
                int suboption = -1;
                while (suboption != 0) {
                    clearTerminal();
                    connectionSettingsMenu();
                    std::cout << "Choose an option: ";
                    std::cin >> suboption;
                    switch (suboption) {
                        case 1:
                            std::cout << "Change Destination Port..." << std::endl;
                            break;
                        case 2:
                            std::cout << "Change Current Port..." << std::endl;
                            try {
                                Services::changeCurrentPort(server);
                            } catch(const std::exception& e) {
                                std::cerr << "Error: " << e.what() << std::endl;
                            }
                            break;
                        case 3:
                            std::cout << "Checking connection..." << std::endl;
                            Services::checkConnection(server);
                            break;
                        case 0:
                            break;
                        default:
                            std::cout << "Invalid option!" << std::endl;
                    }
                }
                break;
            }
            case 5: {
                int suboption = -1;
                while (suboption != 0) {
                    clearTerminal();
                    userSettingsMenu();
                    std::cout << "Choose an option: ";
                    std::cin >> suboption;
                    switch (suboption) {
                        case 1:
                            std::cout << "Edit username..." << std::endl;
                            break;
                        case 2:
                            std::cout << "Generating RSA keys..." << std::endl;
                            break;
                        case 0:
                            break;
                        default:
                            std::cout << "Invalid option!" << std::endl;
                    }
                }
                break;
            }
            case 0:
                std::cout << "Exiting..." << std::endl;
                break;
            default:
                std::cout << "Invalid option!" << std::endl;
        }
        if(option != 0) {
            std::cout << "\nPress ENTER to continue...";
            std::cin.ignore();
            std::cin.get();
        }
    }
    return 0;
}