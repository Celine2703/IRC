/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:06 by ranki             #+#    #+#             */
/*   Updated: 2024/04/17 18:57:28 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void PRIVMSGPosCommand(std::string cmd, std::string tofind, std::string &str)
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

std::string tokenizationPrivateMessageCommand(std::string &cmd, std::vector<std::string> &tmp)
{
	std::stringstream ss(cmd);
	std::string str, msg;
	int count = 2;
	while (ss >> str && count--)
		tmp.push_back(str);
	if (tmp.size() != 2)
		return std::string("");
	PRIVMSGPosCommand(cmd, tmp[1], msg);
	return msg;
}

std::string Server::initialSplitAndValidate(std::string cmd, std::vector<std::string> &tmp)
{
	std::string str = tokenizationPrivateMessageCommand(cmd, tmp);
	if (tmp.size() != 2)
	{
		tmp.clear();
		return std::string("");
	}
	tmp.erase(tmp.begin());
	return tmp[0];
}

void Server::splitStringByComma(std::string str1, std::vector<std::string> &tmp)
{
	std::string str2;
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
}

void Server::removeEmptyStrings(std::vector<std::string> &tmp)
{
	for (size_t i = 0; i < tmp.size(); i++)
	{
		if (tmp[i].empty())
			tmp.erase(tmp.begin() + i--);
	}
}

std::string Server::adjustOriginalString(std::string str)
{
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

std::string getMessageAfterColon(std::string input)
{
	size_t colonPos = input.find(':');
	if (colonPos != std::string::npos)
	{
		return input.substr(colonPos + 1);
	}
	return "";
}

std::string Server::SplitCmdPrivmsg(std::string cmd, std::vector<std::string> &tmp)
{
	std::string message = getMessageAfterColon(cmd);
	std::string str1 = initialSplitAndValidate(cmd, tmp);
	if (str1.empty())
		return std::string("");

	tmp.clear();
	splitStringByComma(str1, tmp);
	removeEmptyStrings(tmp);
	
	return message;
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

bool Server::validateMessageAndRecipients(const std::vector<std::string> &tmp, const std::string &message, int fd)
{
	if (tmp.empty())
	{
		senderror(411, findClientByFd(fd)->getNickname(), fd, " :No recipient given (PRIVMSG)\n");
		return false;
	}
	if (message.empty())
	{
		senderror(412, findClientByFd(fd)->getNickname(), fd, " :No text to send\n");
		return false;
	}
	if (tmp.size() > 10)
	{
		senderror(407, findClientByFd(fd)->getNickname(), fd, " :Too many recipients\n");
		return false;
	}
	return true;
}

void Server::sendMessageToChannel(const std::string &channel, const std::string &message, int fd)
{
	std::string nickname = findClientByFd(fd)->getNickname();
	std::string username = findClientByFd(fd)->getUsername();
	std::string resp = ":" + nickname + "!~" + username + "@localhost PRIVMSG #" + channel + " :" + message + "\n";
	findChannelByName(channel)->sendToAll(resp, fd);
}

void Server::sendMessageToUser(const std::string &user, const std::string &message, int fd)
{
	std::string nickname = findClientByFd(fd)->getNickname();
	std::string username = findClientByFd(fd)->getUsername();
	std::string resp = ":" + nickname + "!~" + username + "@localhost PRIVMSG " + user + " :" + message + "\n";
	sendResponse(resp, findClientByNick(user)->getFd());
}

void Server::distributeMessages(const std::vector<std::string> &recipients, const std::string &message, int fd)
{
	for (size_t i = 0; i < recipients.size(); ++i)
	{
		if (recipients[i][0] == '#')
		{
			std::string channelName = recipients[i].substr(1);
			sendMessageToChannel(channelName, message, fd);
		}
		else
		{
			sendMessageToUser(recipients[i], message, fd);
		}
	}
}

void Server::PRIVMSG(std::string cmd, int fd)
{
	std::vector<std::string> tmp;
	std::string message = SplitCmdPrivmsg(cmd, tmp);

	if (!validateMessageAndRecipients(tmp, message, fd))
		return;


	distributeMessages(tmp, message, fd);

}
