/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 20:25:45 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 20:58:37 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void FindK(std::string cmd, std::string tofind, std::string &str)
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

std::string SplitCmdK(std::string &cmd, std::vector<std::string> &tmp)
{
    std::stringstream ss(cmd);
    std::string str, reason;
    int count = 3;
    while (ss >> str && count--)
        tmp.push_back(str);
    if (tmp.size() != 3)
        return std::string("");
    FindK(cmd, tmp[2], reason);
    return reason;
}

std::string Server::ExtractReason(std::string& reason)
{
    if (reason[0] == ':')
    {
        reason.erase(reason.begin());
    }
    else
    {
        size_t pos = reason.find(' ');
        if (pos != std::string::npos)
            reason = reason.substr(0, pos);
    }
    return reason;
}

void Server::CleanChannelNames(std::vector<std::string>& tmp, int fd)
{
    for (size_t i = 0; i < tmp.size(); i++)
    {
        if (tmp[i].empty())
        {
            tmp.erase(tmp.begin() + i--);
        }
        else if (*(tmp[i].begin()) == '#')
        {
            tmp[i].erase(tmp[i].begin());
        }
        else
        {
            senderror(403, GetClient(fd)->getNickname(), tmp[i], GetClient(fd)->getFD(), " :No such channel\r\n");
            tmp.erase(tmp.begin() + i--);
        }
    }
}

void Server::SplitChannelNames(const std::string& str, std::vector<std::string>& tmp)
{
    std::string str1;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ',')
        {
            tmp.push_back(str1);
            str1.clear();
        }
        else
        {
            str1 += str[i];
        }
    }
    tmp.push_back(str1);
}

std::string Server::SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
        std::string reason = SplitCmdK(cmd, tmp);

        if (tmp.size() < 3)
            return std::string("");

        tmp.erase(tmp.begin());
        std::string str = tmp[0];
        user = tmp[1];
        tmp.clear();

        SplitChannelNames(str, tmp);
        CleanChannelNames(tmp, fd);
        reason = ExtractReason(reason);

        return reason;
    }

void Server::KICK(std::string cmd, int fd)
{
    // ERR_BADCHANMASK (476) // if the channel mask is invalid
    std::vector<std::string> tmp;
    std::string reason, user;
    reason = SplitCmdKick(cmd, tmp, user, fd);
    if (user.empty())
    {
        senderror(461, GetClient(fd)->getNickname(), GetClient(fd)->getFD(), " :Not enough parameters\r\n");
        return;
    }
    for (size_t i = 0; i < tmp.size(); i++)
    { // search for the channel
        if (GetChannel(tmp[i]))
        { // check if the channel exist
            Channel *ch = GetChannel(tmp[i]);
            if (!ch->get_Client(fd) && !ch->get_admin(fd)) // check if the client is in the channel
            {
                senderror(442, GetClient(fd)->getNickname(), "#" + tmp[i], GetClient(fd)->getFD(), " :You're not on that channel\r\n");
                continue;
            }
            if (ch->get_admin(fd))
            { // check if the client is admin
                if (ch->GetClientInChannel(user))
                { // check if the client to kick is in the channel
                    std::stringstream ss;
                    ss << ":" << GetClient(fd)->getNickname() << "!~" << GetClient(fd)->getUsername() << "@"
                       << "localhost"
                       << " KICK #" << tmp[i] << " " << user;
                    if (!reason.empty())
                        ss << " :" << reason << "\r\n";
                    else
                        ss << "\r\n";
                    ch->sendTo_all(ss.str());
                    if (ch->get_admin(ch->GetClientInChannel(user)->getFD()))
                        ch->remove_admin(ch->GetClientInChannel(user)->getFD());
                    else
                        ch->remove_Client(ch->GetClientInChannel(user)->getFD());
                    if (ch->GetClientsNumber() == 0)
                        channels.erase(channels.begin() + i);
                }
                else // if the client to kick is not in the channel
                {
                    senderror(441, GetClient(fd)->getNickname(), "#" + tmp[i], GetClient(fd)->getFD(), " :They aren't on that channel\r\n");
                    continue;
                }
            }
            else // if the client is not admin
            {
                senderror(482, GetClient(fd)->getNickname(), "#" + tmp[i], GetClient(fd)->getFD(), " :You're not channel operator\r\n");
                continue;
            }
        }
        else // if the channel doesn't exist
            senderror(403, GetClient(fd)->getNickname(), "#" + tmp[i], GetClient(fd)->getFD(), " :No such channel\r\n");
    }
}