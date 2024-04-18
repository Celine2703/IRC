/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationClient.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:20 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 14:27:40 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::sendResponse(std::string response, int fd)
{
	std::cout << "Envoie => " << fd << " => " << response << std::endl;
	if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}

void Server::receiveData(int fd)
{
	char buffer[1024] = {0};
	int bytes = recv(fd, buffer, sizeof(buffer), 0);
	Client *cli = findClientByFd(fd);
	if (bytes <= -1)
		return ;
	if (bytes == 0)
	{
		clearClients(fd);
		close(fd);
	}
	else
	{
		buffer[bytes] = '\0';

		// on recupere la sortie Client
		cli->setBuffer((std::string)buffer);

		if (cli->getBuffer().find("\n") == std::string::npos)
			return;

		std::cout << "Received in Client : " << cli->getBuffer() << std::endl;

		std::istringstream iss(buffer);
		std::string line;
		while (std::getline(iss, line))
		{
			std::cout << "Execute commande : " << line << std::endl;
			parseCommand(line, fd);
		}

		if (findClientByFd(fd))
		{
			findClientByFd(fd)->clearBuffer();
		}
	}
}

// Fonction pour envoyer un message d'erreur à un Client
void Server::senderror(int code, std::string Clientname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << Clientname << msg;
	std::string resp = ss.str();
	if (send(fd, resp.c_str(), resp.size(), 0) == -1)
		std::cerr << "send() faild" << std::endl;
}

// Fonction pour envoyer un message d'erreur à un Client concernant un canal spécifique
void Server::senderror(int code, std::string Clientname, std::string channelname, int fd, std::string msg)
{
	std::stringstream ss;
	ss << ":localhost " << code << " " << Clientname << " " << channelname << msg;
	std::string resp = ss.str();
	if (send(fd, resp.c_str(), resp.size(), 0) == -1)
		std::cerr << "send() faild" << std::endl;
}