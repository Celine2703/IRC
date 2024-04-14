#ifndef PART_HPP
#define PART_HPP

#include "../command.hpp"

class Part : public Commands {
public:
    // Constructor
    Part();

    // Destructor
    ~Part();

    // Copy assignment operator
    Part& operator=(const Part& other);

    // Copy constructor
    Part(const Part& other);

    // Execute function
    void execute();

private:
    // Add any private members here
};

#endif // PART_HPP