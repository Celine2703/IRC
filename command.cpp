#include "command.hpp"

int identify(std::string line) {
    if (line.find("NICK") == 0) {
        return kNick;
    }
    if (line.find("USER") == 0) {
        return kUser;
    }
    if (line.find("JOIN") == 0) {
        return kJoin;
    }
    if (line.find("PART") == 0) {
        return kPart;
    }
    if (line.find("PRIVMSG") == 0) {
        return kPrivateMessage;
    }
    if (line.find("QUIT") == 0) {
        return kQuit;
    }
    throw std::runtime_error("Unknown command");
}
