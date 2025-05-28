#include "services.hpp"
#include "../config/config.hpp"
#include <httplib.h>
#include <gmp.h>

void Services::connectToTargetAsConfig() {
    int port = Config::Server::PORT;
    std::string host = Config::Server::HOST;
    std::cout << "Trying Connecting to target server at " << Config::get_target_url() << std::endl;
}

void Services::connectToTargetDirect(int port) {
    std::string host = Config::Server::HOST;
    std::cout << "Trying Connecting to target server at " << host << ":" << port << std::endl;
}

void Services::connectToTargetDirect(int port, const std::string& host) {
    
}

void Services::disconnectFromServer(AppServer& server) {

}

void Services::checkServerStatus(AppServer& server) {
    httplib::Client client(Config::Server::HOST, server.getPort());

    if(server.status() == Config::ServerStatus::ONLINE) {
        std::cout << "Connected to server at port: " << server.getPort() << std::endl;
        if(auto res = client.Get("/checkConnection")) {
            if(res->status == 200) {
                std::cout << "Server is running." << std::endl;
            } else {
                std::cout << "Server is not responding, but status is ONLINE." << std::endl;
            }
        } else {
            std::cout << "Error connecting to server " << std::endl;
        }
    } else {
        std::cout << "Not connected to server." << std::endl;
        if(auto res = client.Get("/checkConnection")) {
            if(res->status == 200) {
                std::cout << "Server is running, but status is OFFLINE." << std::endl;
            } else {
                std::cout << "Server is not responding." << std::endl;
            }
        } else {
            std::cout << "Error connecting to server " << std::endl;
        }
    }
}

void Services::startServerMode(AppServer& server) {
    std::cout << "Starting server mode..." << std::endl;
    try {
        server.start();
        std::cout << "Server started on port: " << server.getPort() << std::endl;
    } catch(const std::exception& e) {
        std::cerr << "Error starting server: " << e.what() << std::endl;
    }
}

void Services::stopServerMode(AppServer& server) {
    std::cout << "Stopping server mode..." << std::endl;
    try {
        server.stop();
        std::cout << "Server stopped." << std::endl;
        // Atualiza o port do AppServer após parar, para sincronizar com o config
        server.updatePort(Config::Server::PORT);
        std::cout << "Server port updated to: " << server.getPort() << std::endl;
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
    std::cout << "Current configured port: " << Config::Server::PORT;
    if(Config::Server::PORT == Config::Default::PORT) {
        std::cout << " (Default)" << std::endl;
    } else {
        std::cout << std::endl;
    }
    std::cout << "Server port: ";
    if(server.status() == Config::ServerStatus::ONLINE) {
        std::cout << server.getPort() << std::endl;
    } else {
        std::cout << "Server not started." << std::endl;
    }
    int new_port;
    std::cout << "Enter new port: ";
    while (!(std::cin >> new_port)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Enter a valid port: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "CHEGA AQUI" << new_port << std::endl;
    Config::set_port(new_port);

    if(server.status() == Config::ServerStatus::OFFLINE) {
        server.updatePort(new_port);
        std::cout << "Server port updated to: " << server.getPort() << std::endl;
    }

    if(server.status() == Config::ServerStatus::ONLINE && server.getPort() != Config::Server::PORT) {
        std::cout << "Server port is different from current port." << std::endl;
        char choice;
        do {
            std::cout << "Update server port? (y/n): ";
            std::cin >> choice;
            if (choice == 'y' || choice == 'Y') {
                server.updatePort(Config::Server::PORT);
                std::cout << "Server port updated to: " << server.getPort() << std::endl;
            } else {
                std::cout << "Server port not updated." << std::endl;
            }
        } while(choice != 'y' && choice != 'n');
    }
    std::cout << "Port updated: " << Config::Server::PORT << std::endl;
}

void Services::changeTargetPort(AppServer& server) {
    std::cout << "Current target port: " << Config::Server::TARGER_PORT;
    if(Config::Server::TARGER_PORT == Config::Default::TARGET_PORT) {
        std::cout << " (Default)" << std::endl;
    } else {
        std::cout << std::endl;
    }
    std::cout << "Enter new target port: ";
    int new_port;
    while (!(std::cin >> new_port)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Enter a valid port: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Config::set_target_port(new_port);
    std::cout << "Target port updated to: " << Config::Server::TARGER_PORT << std::endl;
}

void Services::editUsername(Chat& chat) {
    std::cout << "Current username: " << Config::User::USERNAME << std::endl;
    std::cout << "Enter new username: ";
    std::string new_username;
    std::cin >> new_username;
    try {
        Config::set_username(new_username);
        std::cout << "Username updated to: " << Config::User::USERNAME << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error updating username: " << e.what() << std::endl;
    }
    chat.changeUsernames(Config::User::USERNAME);
}

void Services::generateRSAKeys() {
    std::cout << "Generating RSA keys..." << std::endl;
    try {
        RSA::Keys keys = RSA::generate_keys();
        Config::User::PUBLIC_KEY.n = keys.public_key.n;
        Config::User::PUBLIC_KEY.e = keys.public_key.e;
        Config::User::PRIVATE_KEY.n = keys.private_key.n;
        Config::User::PRIVATE_KEY.d = keys.private_key.d;
    } catch(const std::exception& e) {
        std::cerr << "Error generating RSA keys: " << e.what() << std::endl;
    }

}

void Services::showPortsInfo(AppServer& server) {
    std::cout << "\n--- Ports Info ---" << std::endl;
    std::cout << "Config::Server::PORT: " << Config::Server::PORT << std::endl;
    std::cout << "Config::Server::TARGER_PORT: " << Config::Server::TARGER_PORT << std::endl;
    std::cout << "Server (runtime) port: " << server.getPort() << std::endl;
}

void Services::generateUserId() {
    std::stringstream ss;
    std::random_device rd;
    for(int i = 0; i < 16; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << rd() % 256;
    }
    Config::User::USER_ID = ss.str();
}