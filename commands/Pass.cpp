/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:03 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 17:52:04 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <poll.h>
#include <signal.h>
#include "../includes/client.hpp"

std::string removeNewline2(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	return str;
}

// PASS
void Server::PASS_client(int fd, std::string cmd)
{
	Client *cli = GetClient(fd);

	if (cmd.empty())
		sendResponse(ERR_NOTENOUGHPARAM(std::string("*")), fd);
	else if (!cli->isRegistered())
	{
		std::string pass = cmd;
		if (removeNewline2(pass) == password)
		{
			cli->setRegistered(true);
		}
		else
			sendResponse(ERR_INCORPASS(std::string("*")), fd);
	}
	else
		sendResponse(ERR_ALREADYREGISTERED(GetClient(fd)->getNickname()), fd);
}