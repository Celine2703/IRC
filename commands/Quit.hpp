#ifndef QUIT_HPP
#define QUIT_HPP

#include "../commands.hpp"

class Quit : public Commands {
public:
    Quit() {
        // Constructor implementation
    }

    ~Quit() {
        // Destructor implementation
    }

    Quit(const Quit& other) {
        // Copy constructor implementation
    }

    Quit& operator=(const Quit& other) {
        // Copy assignment operator implementation
        return *this;
    }

    void execute() {
        // execute() function implementation
    }
};

#endif // QUIT_HPP