#include "services.hpp"


#ifdef _WIN32
#include <windows.h>
#define CLEAR_COMMAND "cls"
#else
#define CLEAR_COMMAND "clear"
#endif

void Services::connectToTargetAsConfig() {
    std::cout << "Connecting to target server at " << Config::get_target_url() << std::endl;
}

void Services::connectToTargetDirect(int port) {
    std::cout << "Connecting to target server at " << Config::Server::HOST << ":" << port << std::endl;
}

void Services::connectToTargetDirect(int port, const std::string& host) {
    std::cout << "Connecting to target server at " << host << ":" << port << std::endl;
}

void Services::disconnectFromServer(AppServer& server) {
    server.stop();
    std::cout << "Disconnected from server." << std::endl;
}

void Services::checkServerStatus(AppServer& server) {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "     *  SERVER STATUS CHECK  *     "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    httplib::Client client(Config::Server::HOST, server.getPort());
    if (server.status() == Config::ServerStatus::ONLINE) {
        std::cout << TerminalStyle::GREEN << "Connected to server at port: " << server.getPort() << TerminalStyle::RESET << std::endl;
        if (auto res = client.Get("/checkConnection")) {
            std::cout << (res->status == 200 ? TerminalStyle::GREEN + std::string("Server is running.") : TerminalStyle::RED + std::string("Server is not responding, but status is ONLINE.")) << TerminalStyle::RESET << std::endl;
        } else {
            std::cout << TerminalStyle::RED << "Error connecting to server" << TerminalStyle::RESET << std::endl;
        }
    } else {
        std::cout << TerminalStyle::RED << "Not connected to server." << TerminalStyle::RESET << std::endl;
        if (auto res = client.Get("/checkConnection")) {
            std::cout << (res->status == 200 ? TerminalStyle::YELLOW + std::string("Server is running, but status is OFFLINE.") : TerminalStyle::RED + std::string("Server is not responding.")) << TerminalStyle::RESET << std::endl;
        } else {
            std::cout << TerminalStyle::RED << "Error connecting to server" << TerminalStyle::RESET << std::endl;
        }
    }
    std::cout << TerminalStyle::YELLOW << "\n+--------------------------------------+" << TerminalStyle::RESET << std::endl;
}

void Services::startServerMode(AppServer& server) {
    try {
        server.start();
        std::cout << "Server started on port: " << server.getPort() << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
    }
}

void Services::stopServerMode(AppServer& server) {
    try {
        server.stop();
        server.updatePort(Config::Server::PORT);
        std::cout << "Server stopped. Port updated to: " << server.getPort() << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error stopping server: " << e.what() << std::endl;
    }
}

void Services::startClientMode() {
    std::cout << "Starting client mode (chat)..." << std::endl;
    // Implement client mode logic here
}

// Function to change the current server port
void Services::changeCurrentPort(AppServer& server) {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "     *  CHANGE CURRENT PORT  *     "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::BOLD << "Current configured port: " << TerminalStyle::CYAN << Config::Server::PORT << TerminalStyle::RESET << (Config::Server::PORT == Config::Default::PORT ? TerminalStyle::YELLOW + std::string(" (Default)") : "") << TerminalStyle::RESET << std::endl;
    if(server.status() == Config::ServerStatus::ONLINE)
        std::cout << TerminalStyle::BOLD << "Server port: " << TerminalStyle::CYAN << server.getPort() << TerminalStyle::RESET << std::endl;
    else
        std::cout << TerminalStyle::RED << "Server not started." << TerminalStyle::RESET << std::endl;
    int new_port;
    std::cout << TerminalStyle::BOLD << "Enter new port: " << TerminalStyle::RESET;
    while (!(std::cin >> new_port)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << TerminalStyle::RED << "Invalid input. Enter a valid port: " << TerminalStyle::RESET;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Config::set_port(new_port);
    if(server.status() == Config::ServerStatus::OFFLINE) {
        server.updatePort(new_port);
        std::cout << TerminalStyle::GREEN << "Server port updated to: " << server.getPort() << TerminalStyle::RESET << std::endl;
    } else if(server.getPort() != Config::Server::PORT) {
        char choice;
        do {
            std::cout << TerminalStyle::BOLD << "Update server port? (y/n): " << TerminalStyle::RESET;
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                server.updatePort(Config::Server::PORT);
                std::cout << TerminalStyle::GREEN << "Server port updated to: " << server.getPort() << TerminalStyle::RESET << std::endl;
            } else if (choice == 'n' || choice == 'N') {
                std::cout << TerminalStyle::YELLOW << "Server port not updated." << TerminalStyle::RESET << std::endl;
            }
        } while(choice != 'y' && choice != 'Y' && choice != 'n' && choice != 'N');
    }
    std::cout << TerminalStyle::GREEN << "Port updated: " << Config::Server::PORT << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::YELLOW << "\n+--------------------------------------+" << TerminalStyle::RESET << std::endl;
}

void Services::changeTargetPort(AppServer& server) {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "    *  CHANGE DESTINATION PORT  *   "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::BOLD << "Current target port: " << TerminalStyle::CYAN << Config::Server::TARGER_PORT << TerminalStyle::RESET << (Config::Server::TARGER_PORT == Config::Default::TARGET_PORT ? TerminalStyle::YELLOW + std::string(" (Default)") : "") << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::BOLD << "Enter new target port: " << TerminalStyle::RESET;
    int new_port;
    while (!(std::cin >> new_port)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << TerminalStyle::RED << "Invalid input. Enter a valid port: " << TerminalStyle::RESET;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Config::set_target_port(new_port);
    std::cout << TerminalStyle::GREEN << "Target port updated to: " << Config::Server::TARGER_PORT << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::YELLOW << "\n+--------------------------------------+" << TerminalStyle::RESET << std::endl;
}

void Services::editUsername(Chat& chat) {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "        *  EDIT USERNAME  *        "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::BOLD << "Current username: " << TerminalStyle::CYAN << Config::User::USERNAME << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::BOLD << "Enter new username: " << TerminalStyle::RESET;
    std::string new_username;
    std::cin >> new_username;
    try {
        Config::set_username(new_username);
        std::cout << TerminalStyle::GREEN << "Username updated to: " << Config::User::USERNAME << TerminalStyle::RESET << std::endl;
    } catch (const std::exception& e) {
        std::cerr << TerminalStyle::RED << "Error updating username: " << e.what() << TerminalStyle::RESET << std::endl;
    }
    chat.changeUsernames(Config::User::USERNAME);
    std::cout << TerminalStyle::YELLOW << "\n+--------------------------------------+" << TerminalStyle::RESET << std::endl;
}

void Services::generateRSAKeys() {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "      *  GENERATE RSA KEYS  *      "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::BOLD << "Generating RSA keys..." << TerminalStyle::RESET << std::endl;
    try {
        RSA::Keys keys = RSA::generate_keys();
        Config::User::PUBLIC_KEY.n = keys.public_key.n;
        Config::User::PUBLIC_KEY.e = keys.public_key.e;
        Config::User::PRIVATE_KEY.n = keys.private_key.n;
        Config::User::PRIVATE_KEY.d = keys.private_key.d;
        std::cout << TerminalStyle::GREEN << "RSA keys generated successfully!" << TerminalStyle::RESET << std::endl;
    } catch(const std::exception& e) {
        std::cerr << TerminalStyle::RED << "Error generating RSA keys: " << e.what() << TerminalStyle::RESET << std::endl;
    }
    std::cout << TerminalStyle::YELLOW << "\n+--------------------------------------+" << TerminalStyle::RESET << std::endl;
}

void Services::showPortsInfo(AppServer& server) {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "        *  PORTS INFO  *         "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n";
    std::cout << TerminalStyle::BOLD << "Config::Server::PORT: " << TerminalStyle::CYAN << Config::Server::PORT << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::BOLD << "Config::Server::TARGER_PORT: " << TerminalStyle::CYAN << Config::Server::TARGER_PORT << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::BOLD << "Server (runtime) port: " << TerminalStyle::CYAN << server.getPort() << TerminalStyle::RESET << std::endl;
    std::cout << TerminalStyle::YELLOW << "\n+--------------------------------------+" << TerminalStyle::RESET << std::endl;
}

void Services::generateUserId() {
    std::stringstream ss;
    std::random_device rd;
    for(int i = 0; i < 16; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << rd() % 256;
    }
    Config::User::USER_ID = ss.str();
}

void Services::clearTerminal() {
    std::system(CLEAR_COMMAND);
}