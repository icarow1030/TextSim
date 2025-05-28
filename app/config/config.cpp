#include "config.hpp"
#include <string>

namespace Config {
    namespace User {
        std::string USERNAME = Default::USERNAME;
    }
    namespace Server {
        int PORT = Default::PORT;
        int TARGER_PORT = Default::TARGET_PORT;
    }
}