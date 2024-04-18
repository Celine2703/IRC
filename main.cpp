/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:00 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 16:36:20 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"
#include <csignal>

void usage()
{
    std::cout << "Usage: ircserv <port> <password>" << std::endl;
}

void signalHandler(int signum) {
    std::cout << "Interrupt signal (" << signum << ") received.\n";

    Server::SetserverRunning(false);
}

int main(int argc, char **argv)
{
    // if (argc != 3) {
    //     usage();
    //     return 1;
    // }
    (void)argc;
    (void)argv;
    std::cout << std::endl
              << GRE << "*** SERVER *** \n"
              << WHI << std::endl;
    std::string password = "1"; // normalement donner par l'user  argv[2]
    const char *port = "6667";  // normalement donner par l'user  argv[1]
    if (argc == 3)
    {
        password = argv[2];
        port = argv[1];
    }
    Server s;
    signal(SIGINT, signalHandler);
    try
    {
        
        s.Start(password, std::atoi(port));
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}