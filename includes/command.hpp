/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:39 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 17:51:40 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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


