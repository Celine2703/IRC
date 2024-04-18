/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:22 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 11:52:24 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Channel *Server::findChannelByName(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (removeAllNewLines(this->channels[i].GetName()) == removeAllNewLines(name))
		{
			return &channels[i];
		}
	}
	return NULL;
}

void Server::updateClientNickname(std::string &oldNick, std::string &newNick)
{
	for (std::vector<Channel>::iterator itChannel = channels.begin(); itChannel != channels.end(); ++itChannel)
	{

		for (std::vector<Client>::iterator itClient = itChannel->getClients().begin(); itClient != itChannel->getClients().end(); ++itClient)
		{
			if (itClient->getNickname() == oldNick)
			{
				itClient->setNickname(newNick);
			}
		}

		for (std::vector<Client>::iterator itAdmin = itChannel->getAdmins().begin(); itAdmin != itChannel->getAdmins().end(); ++itAdmin)
		{
			if (itAdmin->getNickname() == oldNick)
			{
				itAdmin->setNickname(newNick);
			}
		}
	}
}