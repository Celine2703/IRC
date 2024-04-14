/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 18:39:22 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 18:44:04 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"



std::string Server::tTopic()
{
	std::time_t current = std::time(NULL);
	std::stringstream res;

	res << current;
	return res.str();
}
std::string Server::gettopic(std::string &input)
{
	size_t pos = input.find(":");
	if (pos == std::string::npos)
	{
		return "";
	}
	return input.substr(pos);
}

int Server::getpos(std::string &cmd)
{
	for (int i = 0; i < (int)cmd.size(); i++)
		if (cmd[i] == ':' && (cmd[i - 1] == 32))
			return i;
	return -1;
}

void Server::TOPIC(std::string &cmd, int &fd)
{
	if (cmd == "TOPIC :")
		{senderror(461, GetClient(fd)->getNickname(), fd, " :Not enough parameters\r\n");return;} // ERR_NEEDMOREPARAMS (461) if there are not enough parameters
	std::vector<std::string> scmd = tokenizationCommand(cmd);
	if (scmd.size() == 1)
		{senderror(461, GetClient(fd)->getNickname(), fd, " :Not enough parameters\r\n");return;} // ERR_NEEDMOREPARAMS (461) if there are not enough parameters
	std::string nmch = scmd[1].substr(1);
	if (!GetChannel(nmch)) // ERR_NOSUCHCHANNEL (403) if the given channel does not exist
		{senderror(403, "#" + nmch, fd, " :No such channel\r\n"); return;}
	if (!(GetChannel(nmch)->get_Client(fd)) && !(GetChannel(nmch)->get_admin(fd)))
		{senderror(442, "#" + nmch, fd, " :You're not on that channel\r\n");return;} // ERR_NOTONCHANNEL (442) if the client is not on the channel
	if (scmd.size() == 2)
	{
		if (GetChannel(nmch)->GetTopicName() == "")
		{sendResponse(": 331 " + GetClient(fd)->getNickname() + " " + "#" + nmch + " :No topic is set\r\n", fd);return;} // RPL_NOTOPIC (331) if no topic is set
		size_t pos = GetChannel(nmch)->GetTopicName().find(":");
		if (GetChannel(nmch)->GetTopicName() != "" && pos == std::string::npos)
		{
			sendResponse(": 332 " + GetClient(fd)->getNickname() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);			  // RPL_TOPIC (332) if the topic is set
			sendResponse(": 333 " + GetClient(fd)->getNickname() + " " + "#" + nmch + " " + GetClient(fd)->getNickname() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd); // RPL_TOPICWHOTIME (333) if the topic is set
			return;
		}
		else
		{
			size_t pos = GetChannel(nmch)->GetTopicName().find(" ");
			if (pos == 0)
				GetChannel(nmch)->GetTopicName().erase(0, 1);
			sendResponse(": 332 " + GetClient(fd)->getNickname() + " " + "#" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n", fd);			  // RPL_TOPIC (332) if the topic is set
			sendResponse(": 333 " + GetClient(fd)->getNickname() + " " + "#" + nmch + " " + GetClient(fd)->getNickname() + " " + GetChannel(nmch)->GetTime() + "\r\n", fd); // RPL_TOPICWHOTIME (333) if the topic is set
			return;
		}
	}

	if (scmd.size() >= 3)
	{
		std::vector<std::string> tmp;
		int pos = getpos(cmd);
		if (pos == -1 || scmd[2][0] != ':')
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(scmd[2]);
		}
		else
		{
			tmp.push_back(scmd[0]);
			tmp.push_back(scmd[1]);
			tmp.push_back(cmd.substr(getpos(cmd)));
		}

		if (tmp[2][0] == ':' && tmp[2][1] == '\0')
		{senderror(331, "#" + nmch, fd, " :No topic is set\r\n");return;} // RPL_NOTOPIC (331) if no topic is set

		if (GetChannel(nmch)->Gettopic_restriction() && GetChannel(nmch)->get_Client(fd))
		{senderror(482, "#" + nmch, fd, " :You're Not a channel operator\r\n");return;} // ERR_CHANOPRIVSNEEDED (482) if the client is not a channel operator
		else if (GetChannel(nmch)->Gettopic_restriction() && GetChannel(nmch)->get_admin(fd))
		{
			GetChannel(nmch)->SetTime(tTopic());
			GetChannel(nmch)->SetTopicName(tmp[2]);
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
				rpl = ":" + GetClient(fd)->getNickname() + "!" + GetClient(fd)->getUsername() + "@localhost TOPIC #" + nmch + " :" + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			else
				rpl = ":" + GetClient(fd)->getNickname() + "!" + GetClient(fd)->getUsername() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			GetChannel(nmch)->sendTo_all(rpl);
		}
		else
		{
			std::string rpl;
			size_t pos = tmp[2].find(":");
			if (pos == std::string::npos)
			{
				GetChannel(nmch)->SetTime(tTopic());
				GetChannel(nmch)->SetTopicName(tmp[2]);
				rpl = ":" + GetClient(fd)->getNickname() + "!" + GetClient(fd)->getUsername() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			else
			{
				size_t poss = tmp[2].find(" ");
				GetChannel(nmch)->SetTopicName(tmp[2]);
				if (poss == std::string::npos && tmp[2][0] == ':' && tmp[2][1] != ':')
					tmp[2] = tmp[2].substr(1);
				GetChannel(nmch)->SetTopicName(tmp[2]);
				GetChannel(nmch)->SetTime(tTopic());
				rpl = ":" + GetClient(fd)->getNickname() + "!" + GetClient(fd)->getUsername() + "@localhost TOPIC #" + nmch + " " + GetChannel(nmch)->GetTopicName() + "\r\n"; // RPL_TOPIC (332) if the topic is set
			}
			GetChannel(nmch)->sendTo_all(rpl);
		}
	}
}