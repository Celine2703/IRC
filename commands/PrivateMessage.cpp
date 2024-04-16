/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:06 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 21:51:02 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void FindPM(std::string cmd, std::string tofind, std::string &str)
{
	size_t i = 0;
	for (; i < cmd.size(); i++)
	{
		if (cmd[i] != ' ')
		{
			std::string tmp;
			for (; i < cmd.size() && cmd[i] != ' '; i++)
				tmp += cmd[i];
			if (tmp == tofind)
				break;
			else
				tmp.clear();
		}
	}
	if (i < cmd.size())
		str = cmd.substr(i);
	i = 0;
	for (; i < str.size() && str[i] == ' '; i++)
		;
	str = str.substr(i);
}

std::string SplitCmdPM(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, msg;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if (tmp.size() != 2)
		return std::string("");
	FindPM(cmd, tmp[1], msg);
	return msg;
}

std::string SplitCmdPrivmsg(std::string cmd, std::vector<std::string> &tmp)
{
	std::string str = SplitCmdPM(cmd, tmp);
	if (tmp.size() != 2)
	{
		tmp.clear();
		return std::string("");
	}
	tmp.erase(tmp.begin());
	std::string str1 = tmp[0];
	std::string str2;
	tmp.clear();
	for (size_t i = 0; i < str1.size(); i++)
	{
		if (str1[i] == ',')
		{
			tmp.push_back(str2);
			str2.clear();
		}
		else
			str2 += str1[i];
	}
	tmp.push_back(str2);
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i].empty())
			tmp.erase(tmp.begin() + i--);
	}
	if (str[0] == ':')
		str.erase(str.begin());
	else
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			if (str[i] == ' ')
			{
				str = str.substr(0, i);
				break;
			}
		}
	}
	return str;
}

void Server::checkChannelsForClients(std::vector<std::string> &tmp, int fd)
{
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i][0] == '#')
		{
			tmp[i].erase(tmp[i].begin());
			if (!findChannelByName(tmp[i]))
			{
				senderror(401, "#" + tmp[i], findClientByFd(fd)->getFd(), " :No such nick/channel\n");
				tmp.erase(tmp.begin() + i);
				i--;
			}
			else if (!findChannelByName(tmp[i])->findClientByFdInChannel(findClientByFd(fd)->getNickname()))
			{
				senderror(404, findClientByFd(fd)->getNickname(), "#" + tmp[i], findClientByFd(fd)->getFd(), " :Cannot send to channel\n");
				tmp.erase(tmp.begin() + i);
				i--;
			}
			else
				tmp[i] = "#" + tmp[i];
		}
		else
		{
			if (!findClientByNick(tmp[i]))
			{
				senderror(401, tmp[i], findClientByFd(fd)->getFd(), " :No such nick/channel\n");
				tmp.erase(tmp.begin() + i);
				i--;
			}
		}
	}
}

void Server::PRIVMSG(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string message = SplitCmdPrivmsg(cmd, tmp);
	if (!tmp.size())
	{
		senderror(411, findClientByFd(fd)->getNickname(), findClientByFd(fd)->getFd(), " :No recipient given (PRIVMSG)\n");
		return;
	}
	if (message.empty())
	{
		senderror(412, findClientByFd(fd)->getNickname(), findClientByFd(fd)->getFd(), " :No text to send\n");
		return;
	}
	if (tmp.size() > 10)
	{
		senderror(407, findClientByFd(fd)->getNickname(), findClientByFd(fd)->getFd(), " :Too many recipients\n");
		return;
	}
	checkChannelsForClients(tmp, fd);
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i][0] == '#')
		{
			tmp[i].erase(tmp[i].begin());
			std::string resp = ":" + findClientByFd(fd)->getNickname() + "!~" + findClientByFd(fd)->getUsername() + "@localhost PRIVMSG #" + tmp[i] + " :" + message + "\n";
			findChannelByName(tmp[i])->sendToAll(resp, fd);
		}
		else
		{
			std::string resp = ":" + findClientByFd(fd)->getNickname() + "!~" + findClientByFd(fd)->getUsername() + "@localhost PRIVMSG " + tmp[i] + " :" + message + "\n";
			sendResponse(resp, findClientByNick(tmp[i])->getFd());
		}
	}
}