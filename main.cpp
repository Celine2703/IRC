/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:00 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 17:54:56 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Server.hpp"

void usage()
{
    std::cout << "Usage: ircserv <port> <password>" << std::endl;
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
    const char *port = "6667";             // normalement donner par l'user  argv[1]
    Server s;

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