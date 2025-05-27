#include <httplib.h>
#include "config/config.hpp"
#include "client/AppClient.hpp"
#include "server/AppServer.hpp"
#include "cryptography/rsa.hpp"
#include "services/services.hpp"
#include <gmp.h>
#include "chat/chat.hpp"

#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <memory>

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
    std::cout << "[3] Start Client Mode (Chat)" << std::endl; // Ao ser chamada, define informações do cliente
    std::cout << "[4] Connection Settings" << std::endl;
    std::cout << "[5] User Settings" << std::endl;
    std::cout << "[0] Exit" << std::endl;
}

// Show connection settings submenu
void connectionSettingsMenu() {
    std::cout << "\n-- Connection Settings --" << std::endl;
    std::cout << "[1] Change Destination Port" << std::endl;
    std::cout << "[2] Change Current Port" << std::endl;
    std::cout << "[3] Show Ports Info" << std::endl;
    std::cout << "[4] Check Server Status" << std::endl;
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
    std::cout << "Welcome to " << Config::APP_NAME << " v" << Config::APP_VERSION << " by " << Config::APP_AUTHOR << std::endl;
    Chat chat;
    std::cout << "Initializing chat..." << std::endl;
    AppServer server(chat);
    std::cout << "Server..." << std::endl;
    AppClient client;
    std::cout << "Client..." << std::endl;
    client.setServerPtr(&server); // Set the server pointer in the client

    int option = -1;
    while (option != 0) {
        std::cout << "\nteste1" << std::endl;
        client.generateClient(); // Ensure client is generated with server information
        std::cout << "teste2" << std::endl;
        clearTerminal(); // Clear terminal at each menu display
        showMenu(server);
        std::cout << "Choose an option: ";
        std::cin >> option;
        switch (option) {
            case 1:
                std::cout << (isServerOnline(server) ? "Disconnecting..." : "Connecting...") << std::endl;
                (isServerOnline(server)) ? Services::disconnectFromServer(server) : Services::connectToTargetAsConfig();
                break;
            case 2: 
                std::cout << (isServerOnline(server) ? "Stopping server mode..." : "Starting server mode...") << std::endl;
                (isServerOnline(server)) ? Services::stopServerMode(server) : Services::startServerMode(server);
                break;
            case 3:
                std::cout << "Starting client mode (chat)..." << std::endl;
                chat.chat(client);
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
                            Services::changeTargetPort(server);
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nPress ENTER to continue...";
                            std::cin.get();
                            break;
                        case 2:
                            std::cout << "Change Current Port..." << std::endl;
                            Services::changeCurrentPort(server);
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nPress ENTER to continue...";
                            std::cin.get();
                            break;
                        case 3:
                            std::cout << "Showing ports info..." << std::endl;
                            Services::showPortsInfo(server);
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nPress ENTER to continue...";
                            std::cin.get();
                            break;
                        case 4:
                            std::cout << "Checking server status..." << std::endl;
                            Services::checkServerStatus(server);
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nPress ENTER to continue...";
                            std::cin.get();
                            break;
                        case 0:
                            break;
                        default:
                            std::cout << "Invalid option!" << std::endl;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "\nPress ENTER to continue...";
                            std::cin.get();
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
                            Services::editUsername();
                            break;
                        case 2:
                            std::cout << "Generating RSA keys..." << std::endl;
                            Services::generateRSAKeys();
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
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nPress ENTER to continue...";
            std::cin.get();
        }
    }
    return 0;
}