
#include "chat.hpp"

void Chat::clearTerminal() const {
    std::system(CLEAR_COMMAND);
}

void Chat::addMessage(const json& message, bool is_local) {
    // Evita duplicação se a mensagem já foi adicionada
    static std::string last_message;
    std::string current = message.dump();
    
    if(current != last_message) {
        messages.push_back(message);
        last_message = current;
        printMessages();
        if(!is_local) {
            std::cout << "Type your message (or '/quit' to quit): ";
            std::cout.flush();
        }
    }
}

json Chat::getMessages() const {
    return messages;
}

void Chat::changeUsernames(const std::string& new_username) {
    for(auto& msg : messages) {
        if(msg.contains("username")) {
            msg["username"] = new_username;
        }
    }
}

void Chat::printHeader() const {
    clearTerminal();
    std::cout << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "|" << TerminalStyle::BG_CYAN << "          *  C H A T  *          "
              << "|" << TerminalStyle::RESET << "\n"
              << TerminalStyle::BOLD
              << "+--------------------------------------+" << TerminalStyle::RESET << "\n\n";
}

void Chat::printMessages() const {
    printHeader();
    for(const auto& msg : messages) {
        std::string username = msg.value("username", Config::Default::USERNAME);
        std::string content = msg.value("content", Config::Default::MESSAGE);
        std::string user_color = (username == Config::User::USERNAME)
            ? TerminalStyle::GREEN : TerminalStyle::CYAN;
        std::cout << TerminalStyle::BOLD << user_color
                  << std::left << std::setw(15) << (username + ":")
                  << TerminalStyle::RESET << " " << content << "\n";
    }
    std::cout << TerminalStyle::YELLOW
              << "+--------------------------------------+"
              << TerminalStyle::RESET << "\n";
}

std::string Chat::hashMessage(const json& message) const {
    return SHA256::hash(
        message["user_id"].get<std::string>() + ":" +
        message["username"].get<std::string>() + ":" +
        message["content"].get<std::string>()
    );
}

std::string Chat::encryptAndEncode(const std::string& value, const RSA::PublicKey& key) const {
    if (value.empty()) throw std::runtime_error("Tentativa de criptografar string vazia");
    mpz_class encrypted = RSA::encrypt(value, key);
    size_t count = 0;
    void* bytes = mpz_export(nullptr, &count, 1, 1, 0, 0, encrypted.get_mpz_t());
    if (count == 0 || bytes == nullptr) throw std::runtime_error("Falha ao exportar dados criptografados");
    std::vector<unsigned char> byte_vec(static_cast<unsigned char*>(bytes), static_cast<unsigned char*>(bytes) + count);
    free(bytes);
    std::string b64 = Base64::encode(byte_vec);
    if (b64.empty()) throw std::runtime_error("Falha na codificação Base64");
    return b64;
}

void Chat::encryptMessageFields(json& message, const RSA::PublicKey& key) const {
    message["user_id"] = encryptAndEncode(message["user_id"].get<std::string>(), key);
    message["username"] = encryptAndEncode(message["username"].get<std::string>(), key);
    message["content"] = encryptAndEncode(message["content"].get<std::string>(), key);
    message["pub_key"]["n"] = encryptAndEncode(message["pub_key"]["n"].get<std::string>(), key);
    message["pub_key"]["e"] = encryptAndEncode(message["pub_key"]["e"].get<std::string>(), key);
    message["hash"] = encryptAndEncode(message["hash"].get<std::string>(), key);
}

void Chat::chat(AppClient& client) {
    printMessages();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string input;
    while(true) {
        std::cout << "Type your message (or '/quit' to quit): ";
        std::getline(std::cin, input);
        if(input == "/quit") break;
        if(input.empty()) continue;
        json message;
        message["user_id"] = Config::User::USER_ID;
        message["username"] = Config::User::USERNAME;
        message["content"] = input;
        message["pub_key"] = {
            {"n", Config::User::PUBLIC_KEY.n.get_str()},
            {"e", Config::User::PUBLIC_KEY.e.get_str()}
        };
        message["hash"] = hashMessage(message);
        addMessage(message, true);
        RSA::PublicKey target_pub_key = Config::Server::TARGET_PUBLIC_KEY;
        encryptMessageFields(message, target_pub_key);
        if(client.sendMessage(message)) {
            std::cout << "Message sent!" << std::endl;
        }
    }
}