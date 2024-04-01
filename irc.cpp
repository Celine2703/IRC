#include <iostream>
#include "server.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Server s;
    s.Start();
    return 0;
}