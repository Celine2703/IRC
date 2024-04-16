/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:29 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 20:21:37 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <poll.h>
#include <signal.h>
#include "Client.hpp"
 bool Server::ServerRunning = false;

Server::Server()
{
	Port = 6667;
	ServerSocketFd = -1;
}

void Server::Start(std::string password, int port)
{
	try
	{
		if (port != 0)
		{
			Port = port;
		}
		ServerSocket();
		std::cout << "Server is running on port : " << YEL << Port << WHI << std::endl;
		std::cout << "Server Password is : " << YEL << password << WHI << std::endl
				  << std::endl;
		std::cout << "waiting to accept a connection" << std::endl;

		this->password = password;
		SetserverRunning(true);
		while (Server::getServerRunning())
		{
			
			if ((poll(&PollFds[0], PollFds.size(), 100) == -1) && Server::getServerRunning()) //-> wait for an event
				throw(std::runtime_error("poll() faild"));
			if (!Server::getServerRunning())
				break;
			for (size_t i = 0; i < PollFds.size(); i++)
			{
				if (PollFds[i].revents & POLLIN) //-> check if the event is POLLIN
				{
					if (PollFds[i].fd == ServerSocketFd) //-> check if the event is from the Server socket
					{
						acceptClient();
					}
					else
					{
						try
						{
							receiveData(PollFds[i].fd);
						}
						catch (std::runtime_error &e)
						{
							std::cerr << e.what() << std::endl;
							std::cout << "Client error" << std::endl;
						}
					}
				}
			}
		}
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
		std::cout << "Server is shutting down" << std::endl;
	}
	std::cout << "Server is shutting down correctly" << std::endl;
}

std::string Server::removeFirstBackLine(std::string str)
{
	size_t pos = str.find("\n\r");
	if (pos != std::string::npos)
	{
		str.erase(pos, 1);
	}

	return str;
}

std::string Server::removeAllNewLines(std::string str)
{
	str.erase(remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(remove(str.begin(), str.end(), '\r'), str.end());
	return str;
}

void Server::acceptClient()
{
	Client NewClient;
	struct sockaddr_in ClientAdd;
	socklen_t addlen = sizeof(ClientAdd);

	struct pollfd NewPoll;

	int ClientSocketFd = accept(ServerSocketFd, (struct sockaddr *)&ClientAdd, &addlen);

	if (ClientSocketFd == -1)
	{
		throw(std::runtime_error("faild to accept Client"));
	}

	if (fcntl(ClientSocketFd, F_SETFL, O_NONBLOCK) == -1)
	{
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	}
	NewPoll.fd = ClientSocketFd;
	NewPoll.events = POLLIN;
	NewPoll.revents = 0;

	NewClient.setFd(ClientSocketFd);
	NewClient.setIp(inet_ntoa(ClientAdd.sin_addr));
	this->Clients.push_back(NewClient);
	this->PollFds.push_back(NewPoll);
	std::cout << "Client connected" << std::endl;
}

void Server::ServerSocket()
{
	struct sockaddr_in add;
	struct pollfd NewPoll;

	add.sin_family = AF_INET;		  // set the address family to ipv4
	add.sin_port = htons(this->Port); // convert the port to network byte order
	add.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address

	ServerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //-> create the Server socket
	if (ServerSocketFd == -1)						  //-> check if the socket is created
		throw(std::runtime_error("faild to create socket"));

	int en = 1;
	if (setsockopt(ServerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(ServerSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(ServerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("faild to bind socket"));
	if (listen(ServerSocketFd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = ServerSocketFd; //-> add the Server socket to the pollfd
	NewPoll.events = POLLIN;	 //-> set the event to POLLIN for reading data
	NewPoll.revents = 0;		 //-> set the revents to 0
	PollFds.push_back(NewPoll);	 //-> add the Server socket to the pollfd
}

void Server::serverInit()
{
	try
	{
		ServerSocket();
	}
	catch (std::runtime_error &e)
	{
		std::cerr << e.what() << std::endl;
	}

	std::cout << "Server is running on port " << Port << std::endl;
}

void Server::SetserverRunning(bool value)
{
	ServerRunning = value;
}

bool Server::getServerRunning()
{
	return ServerRunning;
}