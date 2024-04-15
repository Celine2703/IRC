/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:15 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 18:58:01 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

// Constructeur par défaut de la classe Channel
Channel::Channel()
{
	this->invit_only = 0;
	this->topic = 0;
	this->key = 0;
	this->limit = 0;
	this->topic_restriction = false;
	this->name = "";
	this->topic_name = "";
	char charaters[5] = {'i', 't', 'k', 'o', 'l'};
	for (int i = 0; i < 5; i++)
		modes.push_back(std::make_pair(charaters[i], false));
	this->created_at = "";
}

// Destructeur de la classe Channel
Channel::~Channel() {}

// Constructeur par copie de la classe Channel
Channel::Channel(Channel const &src) { *this = src; }

// Opérateur d'affectation de la classe Channel
Channel &Channel::operator=(Channel const &src)
{
	if (this != &src)
	{
		this->invit_only = src.invit_only;
		this->topic = src.topic;
		this->key = src.key;
		this->limit = src.limit;
		this->topic_restriction = src.topic_restriction;
		this->name = src.name;
		this->password = src.password;
		this->created_at = src.created_at;
		this->topic_name = src.topic_name;
		this->Clients = src.Clients;
		this->admins = src.admins;
		this->modes = src.modes;
	}
	return *this;
}

//**************************************************************************************************************************
//**************************************************************************************************************************
//**************************************       GETTERS AND SETTERS    ******************************************************
//**************************************************************************************************************************
//**************************************************************************************************************************

// Setters
void Channel::SetInvitOnly(int invit_only) { this->invit_only = invit_only; }
void Channel::SetTopic(int topic) { this->topic = topic; }
void Channel::SetTime(std::string time) { this->time_creation = time; }
void Channel::SetKey(int key) { this->key = key; }
void Channel::SetLimit(int limit) { this->limit = limit; }
void Channel::SetTopicName(std::string topic_name) { this->topic_name = topic_name; }
void Channel::SetPassword(std::string password) { this->password = password; }
void Channel::SetName(std::string name) { this->name = name; }
void Channel::set_topicRestriction(bool value) { this->topic_restriction = value; }
void Channel::setModeAtindex(size_t index, bool mode) { modes[index].second = mode; }
void Channel::set_createiontime()
{
	std::time_t _time = std::time(NULL);
	std::ostringstream oss;
	oss << _time;
	this->created_at = std::string(oss.str());
}

// Getters
int Channel::GetInvitOnly() { return this->invit_only; }
int Channel::GetTopic() { return this->topic; }
int Channel::GetKey() { return this->key; }
int Channel::GetLimit() { return this->limit; }
int Channel::GetClientsNumber() { return this->Clients.size() + this->admins.size(); }
bool Channel::Gettopic_restriction() const { return this->topic_restriction; }
bool Channel::getModeAtindex(size_t index) { return modes[index].second; }
bool Channel::ClientInChannel(std::string &nick)
{
	for (size_t i = 0; i < Clients.size(); i++)
	{
		if (Clients[i].getNickname() == nick)
			return true;
	}
	for (size_t i = 0; i < admins.size(); i++)
	{
		if (admins[i].getNickname() == nick)
			return true;
	}
	return false;
}
std::string Channel::GetTopicName() { return this->topic_name; }
std::string Channel::GetPassword() { return this->password; }
std::string Channel::GetName() { return this->name; }
std::string Channel::GetTime() { return this->time_creation; }
std::string Channel::get_creationtime() { return created_at; }
std::string Channel::getModes()
{
	std::string mode;
	for (size_t i = 0; i < modes.size(); i++)
	{
		if (modes[i].first != 'o' && modes[i].second)
			mode.push_back(modes[i].first);
	}
	if (!mode.empty())
		mode.insert(mode.begin(), '+');
	return mode;
}
std::string Channel::ClientChannel_list()
{
	std::string list;
	for (size_t i = 0; i < admins.size(); i++)
	{
		list += "@" + admins[i].getNickname();
		if ((i + 1) < admins.size())
			list += " ";
	}
	if (Clients.size())
		list += " ";
	for (size_t i = 0; i < Clients.size(); i++)
	{
		list += Clients[i].getNickname();
		if ((i + 1) < Clients.size())
			list += " ";
	}
	return list;
}
Client *Channel::get_Client(int fd)
{
	for (std::vector<Client>::iterator it = Clients.begin(); it != Clients.end(); ++it)
	{
		if (it->getFD() == fd)
			return &(*it);
	}
	return NULL;
}
Client *Channel::get_admin(int fd)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if (it->getFD() == fd)
			return &(*it);
	}
	return NULL;
}
Client *Channel::GetClientInChannel(std::string name)
{
	for (std::vector<Client>::iterator it = Clients.begin(); it != Clients.end(); ++it)
	{
		if (it->getNickname() == name)
			return &(*it);
	}
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if (it->getNickname() == name)
			return &(*it);
	}
	return NULL;
}

void Channel::add_Client(Client newClient) { Clients.push_back(newClient); }
void Channel::add_admin(Client newClient) { admins.push_back(newClient); }
void Channel::remove_Client(int fd)
{
	for (std::vector<Client>::iterator it = Clients.begin(); it != Clients.end(); ++it)
	{
		if (it->getFD() == fd)
		{
			Clients.erase(it);
			break;
		}
	}
}
void Channel::remove_admin(int fd)
{
	for (std::vector<Client>::iterator it = admins.begin(); it != admins.end(); ++it)
	{
		if (it->getFD() == fd)
		{
			admins.erase(it);
			break;
		}
	}
}
bool Channel::change_ClientToAdmin(std::string &nick)
{
	size_t i = 0;
	for (; i < Clients.size(); i++)
	{
		if (Clients[i].getNickname() == nick)
			break;
	}
	if (i < Clients.size())
	{
		admins.push_back(Clients[i]);
		Clients.erase(i + Clients.begin());
		return true;
	}
	return false;
}

bool Channel::change_adminToClient(std::string &nick)
{
	size_t i = 0;
	for (; i < admins.size(); i++)
	{
		if (admins[i].getNickname() == nick)
			break;
	}
	if (i < admins.size())
	{
		Clients.push_back(admins[i]);
		admins.erase(i + admins.begin());
		return true;
	}
	return false;
}
//---------------//Methods
//---------------//SendToAll
void Channel::sendTo_all(std::string rpl1)
{
	for (size_t i = 0; i < admins.size(); i++)
		if (send(admins[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
			std::cerr << "send() faild" << std::endl;
	for (size_t i = 0; i < Clients.size(); i++)
		if (send(Clients[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
			std::cerr << "send() faild" << std::endl;
}

void Channel::sendTo_all(std::string rpl1, int fd)
{
	for (size_t i = 0; i < admins.size(); i++)
	{
		if (admins[i].getFD() != fd)
			if (send(admins[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
	for (size_t i = 0; i < Clients.size(); i++)
	{
		if (Clients[i].getFD() != fd)
			if (send(Clients[i].getFD(), rpl1.c_str(), rpl1.size(), 0) == -1)
				std::cerr << "send() faild" << std::endl;
	}
}
