#include <httplib.h>
#include "config/config.hpp"
#include "client/AppClient.hpp"
#include "server/AppServer.hpp"
#include "cryptography/rsa.hpp"
#include "services/services.hpp"
#include <gmp.h>
#include "chat/chat.hpp"
#include "chat/style.hpp"

#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <memory>
#include <locale>

#ifdef _WIN32
#include <windows.h>
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
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "          *  M E N U  *          "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::YELLOW << TerminalStyle::BOLD
              << "| USERNAME: " << TerminalStyle::CYAN << Config::User::USERNAME << TerminalStyle::YELLOW << TerminalStyle::BOLD << std::setw(23 - (int)Config::User::USERNAME.size()) << " " << "|" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n\n";
    std::cout << TerminalStyle::BOLD << "[1] " << TerminalStyle::RESET << (isServerOnline(server) ? "Disconnect from Target Port" : "Connect to Target Port") << std::endl;
    std::cout << TerminalStyle::BOLD << "[2] " << TerminalStyle::RESET << (isServerOnline(server) ? "Stop Server Mode" : "Start Server Mode") << std::endl;
    std::cout << TerminalStyle::BOLD << "[3] " << TerminalStyle::RESET << "Start Client Mode (Chat)" << std::endl;
    std::cout << TerminalStyle::BOLD << "[4] " << TerminalStyle::RESET << "Connection Settings" << std::endl;
    std::cout << TerminalStyle::BOLD << "[5] " << TerminalStyle::RESET << "User Settings" << std::endl;
    std::cout << TerminalStyle::BOLD << "[0] " << TerminalStyle::RESET << "Exit" << std::endl;
    std::cout << TerminalStyle::YELLOW << "+--------------------------------------+" << TerminalStyle::RESET << std::endl;
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
    std::setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    // Server instance
    Chat chat;
    AppServer server(chat);
    AppClient client;
    client.setServerPtr(&server); // Set the server pointer in the client

    Services::generateUserId(); // Generate user ID at startup

    int option = -1;
    while (option != 0) {
        std::cout << "\nteste1" << std::endl;
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
                std::cout << "Starting client mode (chat)..." << std::endl; // Ensure client is generated with server information
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
                std::string decrypted;
                mpz_class encrypted;
                int suboption = -1;
                while (suboption != 0) {
                    clearTerminal();
                    userSettingsMenu();
                    std::cout << "Choose an option: ";
                    std::cin >> suboption;
                    switch (suboption) {
                        case 1:
                            std::cout << "Edit username..." << std::endl;
                            Services::editUsername(chat);
                            break;
                        case 2:
                            std::cout << "Generating RSA keys..." << std::endl;
                            Services::generateRSAKeys();
                            std::cout << Config::User::PUBLIC_KEY.n << std::endl;
                            std::cout << Config::User::PUBLIC_KEY.e << std::endl;
                        
                            encrypted = RSA::encrypt("Hello, World!", Config::User::PUBLIC_KEY);
                            std::cout << "Encrypted message: " << encrypted << std::endl;
                            decrypted = RSA::decrypt(encrypted, Config::User::PRIVATE_KEY);
                            std::cout << "Decrypted message: " << decrypted << std::endl;
                            if(option != 0) {
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                                std::cout << "\nPress ENTER to continue...";
                                std::cin.get();
                            }
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