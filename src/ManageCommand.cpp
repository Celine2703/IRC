/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:26 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 17:54:56 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::ParseCommand(std::string cmd, int fd)
{
	if (cmd.empty())
		return;

	if (cmd == "CAP LS" && GetClient(fd)->isFirstMessage())
	{
		GetClient(fd)->setFirstMessage(false);
		return;
	}

	std::vector<std::string> tokens = tokenizationCommand(cmd);

	if (tokens.size() && (tokens[0] == "PASS" || tokens[0] == "pass"))
	{
		this->PASS_Client(fd, tokens[1]);
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
		JOIN_Client(cmd, fd);
	}
	else if (tokens.size() && (tokens[0] == "PRIVMSG" || tokens[0] == "privmsg"))
	{
		PRIVMSG(cmd, fd);
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