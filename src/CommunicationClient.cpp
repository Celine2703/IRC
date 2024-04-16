/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommunicationClient.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:20 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 19:08:02 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::sendResponse(std::string response, int fd)
{
	// std::cout << "Response:\n" << response;
	std::cout << "Envoie => " << fd << " => " << response << std::endl;
	if (send(fd, response.c_str(), response.size(), 0) == -1)
		std::cerr << "Response send() faild" << std::endl;
}

void Server::ReceiveData(int fd)
{
	char buffer[1024];
	int bytes = recv(fd, buffer, sizeof(buffer), 0);
	Client *cli = GetClient(fd);
	if (bytes <= -1)
	{
		perror("DEBUG");
		std::cerr << "error code : " << errno << "\n";
		std::cerr << "fd : " << fd << "\n";
		std::cerr << "bytes " << bytes << "\n";
		std::cerr << "buffer : " << buffer << "\n";
		throw(std::runtime_error("faild to receive data \n"));
	}
	if (bytes == 0)
	{
		std::cout << "Client disconnected" << std::endl;
		ClearClients(fd);
		close(fd);
	}
	else
	{
		buffer[bytes] = '\0';

		// on recupere la sortie Client
		cli->setBuffer((std::string)buffer);

		// on regarde si c'est la fin de l'output du Client (s'il a fait ctrl + d ou enter)
		if (cli->getBuffer().find("\n") == std::string::npos)
			return;

		// s'il a fait enter alors on va s'occuper de la commande dans sa totalité
		std::cout << "Received in Client : " << cli->getBuffer() << std::endl;

		// on va d'abord split la commandee en "/n"
		std::istringstream iss(buffer);
		std::string line;
		while (std::getline(iss, line))
		{
			// on execute la commande
			std::cout << "Execute commande : " << line << std::endl;
			ParseCommand(line, fd);
		}

		// laiser getClient et non la var cli car si c'est l'order Kick la var cli est null
		if (GetClient(fd))
		{
			GetClient(fd)->clearBuffer();
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