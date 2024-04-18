/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:59 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 13:32:31 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <poll.h>
#include <signal.h>
#include "../includes/Client.hpp"

std::string removeNewline(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	return str;
}

bool Server::isValidNickname(std::string &nickname)
{
	if (!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':'))
		return false;
	for (size_t i = 1; i < nickname.size(); i++)
	{
		if (!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '\n')
			return false;
	}
	return true;
}

bool Server::nicknameAlreadyUseByClient(std::string &nickname)
{
	for (size_t i = 0; i < this->Clients.size(); i++)
	{
		if (this->Clients[i].getNickname() == nickname)
			return true;
	}
	return false;
}

// NICK
void Server::setClientNickname(std::string cmd, int fd)
{
	std::string inuse;
	Client *cli = findClientByFd(fd);
	cmd = removeNewline(cmd);
	if (cmd.empty())
	{
		sendResponse(RED + ERR_NOTENOUGHPARAM(std::string("*")) + WHI, fd);
		return;
	}
	if (nicknameAlreadyUseByClient(cmd) && cli->getNickname() != cmd)
	{
		inuse = "*";
		if (cli->getNickname().empty())
		{
			cli->setNickname(inuse);
		}
		sendResponse(RED + ERR_NICKINUSE(std::string(cmd)) + WHI, fd);
		return;
	}
	if (!isValidNickname(cmd))
	{
		sendResponse(RED + ERR_ERRONEUSNICK(std::string(cmd)) + WHI, fd);
		return;
	}
	else
	{
		if (cli && cli->isRegistered())
		{
			std::string oldnick = cli->getNickname();
			cli->setNickname(cmd);
			this->updateClientNickname(oldnick, cmd);
			if (!oldnick.empty() && oldnick != cmd)
			{
				if (oldnick == "*" && !cli->getUsername().empty())
				{
					// cli->setLogedin(true);
					sendResponse(RPL_NICKCHANGE(cli->getNickname(), cmd), fd);
				}
				else
					sendResponse(RPL_NICKCHANGE(oldnick, cmd), fd);
				return;
			}
		}
		else if (cli && !cli->isRegistered())
			sendResponse(RED + ERR_NOTREGISTERED(cmd) + WHI, fd);
	}
	if (cli && cli->isRegistered() && !cli->getNickname().empty() && !cli->getNickname().empty() && cli->getNickname() != "*" && !cli->isLogin())
	{
		cli->setLogin(true);
		sendResponse(RPL_CONNECTED(cli->getNickname()), fd);
		
		sendResponse((std::string)" \n"
		+ "\n\n                 | |  (_)          / /                           | | (_)      \n"
		+ "  _ __ __ _ _ __ | | ___          / /    ___ _ __ ___   __ _ _ __| |_ _ _ __  \n"
		 + " | '__/ _` | '_ \\| |/ / |        / /    / __| '_ ` _ \\ / _` | '__| __| | '_ \\ \n"
		 + " | | | (_| | | | |   <| |       / /    | (__| | | | | | (_| | |  | |_| | | | |\n"
		 + " |_|  \\__,_|_| |_|_|\\_\\_|      /_/      \\___|_| |_| |_|\\__,_|_|   \\__|_|_| |_|\n", fd);

	}
}