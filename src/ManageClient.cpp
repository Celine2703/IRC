/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:24 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 20:21:19 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Client *Server::findClientByNick(std::string nickname)
{
	for (size_t i = 0; i < this->Clients.size(); i++)
	{
		if (this->Clients[i].getNickname() == nickname)
			return &this->Clients[i];
	}
	return NULL;
}

void Server::clearClients(int fd)
{
	for (size_t i = 0; i < Clients.size(); i++)
	{
		if (PollFds[i].fd == fd)
		{
			PollFds.erase(PollFds.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < Clients.size(); i++)
	{
		if (Clients[i].getFd() == fd)
		{
			Clients.erase(Clients.begin() + i);
			break;
		}
	}
}

Client *Server::findClientByFd(int fd)
{
	for (size_t i = 0; i < this->Clients.size(); i++)
	{
		if (this->Clients[i].getFd() == fd)
			return &this->Clients[i];
	}
	return NULL;
}

// Fonction pour rechercher les Clients dans tous les canaux par leur surnom
int Server::findClientByName(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].findClientByFdInChannel(nickname))
			count++;
	}
	return count;
}