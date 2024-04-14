#ifndef PARSER_HPP
# define PARSER_HPP
#endif

#include <iostream>
#include <string>

enum Command {
        kNick,
        kUser,
        kJoin,
        kPart,
        kPrivateMessage,
        kQuit
    };

class Commands
{
    public :
    static int identify(std::string line);
    virtual void execute(std::string line) = 0;
};


// if the line start by the command, return the command


