/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:26 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 10:16:17 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::parseCommand(std::string cmd, int fd)
{
	if (cmd.empty())
		return;

	if (cmd == "CAP LS" && findClientByFd(fd)->isFirstMessage())
	{
		findClientByFd(fd)->setFirstMessage(false);
		return;
	}



	std::vector<std::string> tokens = tokenizationCommand(cmd);

	if (tokens.size() && (tokens[0] != "USER") && (tokens[0] != "user") && (tokens[0] != "PASS") && (tokens[0] != "pass")
			&& (tokens[0] != "NICK") && (tokens[0] != "nick") && !findClientByFd(fd)->isLogin())
	{
		sendResponse(RED + ERR_NOTREGISTERED(cmd) + WHI, fd);
		return ;
	}

	if (tokens.size() && (tokens[0] == "MODE" || tokens[0] == "mode") && findClientByFd(fd)->getFirstMode() <= 1)
	{
		int newMode = findClientByFd(fd)->getFirstMode() + 1;
		findClientByFd(fd)->setFirstMode(newMode);
		return;
	}

	if (tokens.size() && (tokens[0] == "PASS" || tokens[0] == "pass"))
	{
		this->PASS(fd, tokens[1]);
	}
	else if (tokens.size() && (tokens[0] == "NICK" || tokens[0] == "nick"))
	{
		setClientNickname(tokens[1], fd);
	}
	else if (tokens.size() && (tokens[0] == "USER" || tokens[0] == "user"))
	{
		setClientUsername(tokens[1], fd);
	}
	else if (tokens.size() && (tokens[0] == "JOIN" || tokens[0] == "join"))
	{
		JOIN(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "PRIVMSG" || tokens[0] == "privmsg"))
	{
		PRIVMSG(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "TOPIC" || tokens[0] == "topic"))
	{
		TOPIC(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "KICK" || tokens[0] == "kick"))
	{
		KICK(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "PART" || tokens[0] == "part"))
	{
		PART(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "QUIT" || tokens[0] == "quit"))
	{
		QUIT(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "PING" || tokens[0] == "ping"))
	{
		PING(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "INVITE" || tokens[0] == "invite"))
	{
		INVITE(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "MODE" || tokens[0] == "mode"))
	{
		MODE(cmd, fd);
	}
}

std::vector<std::string> Server::tokenizationCommand(std::string &cmd)
{
	std::vector<std::string> vec;
	std::istringstream stm(cmd);
	std::string token;
	while (std::getline(stm, token, ' '))
	{
		if (!token.empty())
		{
			vec.push_back(token);
		}
	}
	return vec;
}