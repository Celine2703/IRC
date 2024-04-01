#include <iostream>
#include "server.hpp"

void usage() {
    std::cout << "Usage: ircserv <port> <password>" << std::endl;
}

int main(int argc, char** argv) {
    // if (argc != 2) {
    //     usage();
    //     return 1;
    // }
    (void)argc;
    (void)argv;
    std::cout << "Hello, World!" << std::endl;
    Server s;
    try {
        s.Start(0);
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}