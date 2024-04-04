#include <iostream>
#include "server.hpp"


void usage() {
    std::cout << "Usage: ircserv <port> <password>" << std::endl;
}

int main(int argc, char** argv) {
    // if (argc != 3) {
    //     usage();
    //     return 1;
    // }
    (void)argc;
    (void)argv;
    std::cout << std::endl << "*** SERVER *** " << std::endl;
    std::string password = "password0901"; // normalement donner par l'user  argv[2]
    const char *port = "6667"; // normalement donner par l'user  argv[1]
    Server s;

    try {
        s.Start(password, std::atoi(port));
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}