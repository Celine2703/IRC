/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:24 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 19:59:18 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Client *Server::GetClientNick(std::string nickname)
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
		if (Clients[i].getFD() == fd)
		{
			Clients.erase(Clients.begin() + i);
			break;
		}
	}
}

Client *Server::GetClient(int fd)
{
	for (size_t i = 0; i < this->Clients.size(); i++)
	{
		if (this->Clients[i].getFD() == fd)
			return &this->Clients[i];
	}
	return NULL;
}

// Fonction pour rechercher les Clients dans tous les canaux par leur surnom
int Server::SearchForClients(std::string nickname)
{
	int count = 0;
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (this->channels[i].GetClientInChannel(nickname))
			count++;
	}
	return count;
}