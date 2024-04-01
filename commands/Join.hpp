#ifndef JOIN_HPP
#define JOIN_HPP

#include "../command.hpp"

class Join : public Commands {
public:
    // Constructor
    Join() {
        // Add any initialization code here
    }

    // Destructor
    ~Join() {
        // Add any cleanup code here
    }

    // Override the execute method from the base class
    void execute() override {
        // Add your implementation for the Join command here
    }
};

#endif // JOIN_HPP