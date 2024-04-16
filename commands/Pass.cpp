/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:03 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 22:40:11 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <poll.h>
#include <signal.h>
#include "../includes/Client.hpp"

std::string cleanLine(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	return str;
}

bool Server::validateCommand(const std::string& cmd, int fd)
{
    if (cmd.empty())
    {
        sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
        return false;
    }
    return true;
}

bool Server::processPassword(Client* cli, const std::string& cmd, int fd)
{
    std::string pass = cmd;
    if (cleanLine(pass) == password)
    {
        cli->setRegistered(true);
        return true;
    }
    else
    {
        sendResponse(ERR_INCORPASS(std::string("*")), fd);
        return false;
    }
}

bool Server::checkRegistration(Client* cli, int fd)
{
    if (!cli->isRegistered())
    {
        return true;
    }
    else
    {
        sendResponse(ERR_ALREADYREGISTERED(cli->getNickname()), fd);
        return false;
    }
}


void Server::PASS(int fd, std::string cmd)
{
    Client* cli = findClientByFd(fd);

    if (!validateCommand(cmd, fd))
        return;

    if (!checkRegistration(cli, fd))
        return;

    processPassword(cli, cmd, fd);
}
