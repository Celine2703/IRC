/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:34 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 10:06:12 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <poll.h>
#include <signal.h>
#include "../includes/Client.hpp"

void Server::setClientUsername(std::string cmd, int fd)
{

	Client *cli = findClientByFd(fd);
	// if ((cli && cmd.size() < 5))
	// {
	// 	sendResponse(ERR_NOTENOUGHPARAM(cli->getNickname()), fd);
	// 	return;
	// }
	if (!cli || !cli->isRegistered())
		sendResponse(RED + ERR_NOTREGISTERED(std::string("*")) + WHI, fd);
	else if (cli && !cli->getUsername().empty())
	{
		sendResponse(RED + ERR_ALREADYREGISTERED(cli->getNickname()) + WHI, fd);
		return;
	}
	else
	{
		cli->setUsername(cmd);
	}
	if (cli && cli->isRegistered() && !cli->getUsername().empty() && !cli->getNickname().empty() && cli->getNickname() != "*" && !cli->isLogin()) 
	{
		cli->setLogin(true);
		sendResponse(RPL_CONNECTED(cli->getNickname()), fd);
	}
}