/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 20:25:45 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 21:04:25 by ranki            ###   ########.fr       */
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
    std::vector<std::string> tmp;
    std::string reason, user;
    reason = SplitCmdKick(cmd, tmp, user, fd);

    if (!CheckParameters(user, fd))
        return;

    ProcessKickForChannel(user, reason, tmp, fd);
}

bool Server::CheckParameters(const std::string& user, int fd)
{
    if (user.empty())
    {
        senderror(461, "", fd, " :Not enough parameters\r\n");
        return false;
    }
    return true;
}

void Server::ProcessKickForChannel(const std::string& user, const std::string& reason, std::vector<std::string>& tmp, int fd)
{
    for (size_t i = 0; i < tmp.size(); i++)
    {
        Channel* ch = GetChannel(tmp[i]);
        if (!ch)
        {
            senderror(403, "#" + tmp[i], fd, " :No such channel\r\n");
            continue;
        }
        if (!ch->get_Client(fd) && !ch->get_admin(fd))
        {
            senderror(442, "#" + tmp[i], fd, " :You're not on that channel\r\n");
            continue;
        }
        if (!ch->get_admin(fd))
        {
            senderror(482, "#" + tmp[i], fd, " :You're not channel operator\r\n");
            continue;
        }
        if (!ch->GetClientInChannel(user))
        {
            senderror(441, "#" + tmp[i], fd, " :They aren't on that channel\r\n");
            continue;
        }

        // Execute kick operation
        std::stringstream ss;
        ss << ":" << GetClient(fd)->getNickname() << "!~" << GetClient(fd)->getUsername() << "@localhost KICK #" << tmp[i] << " " << user;
        if (!reason.empty())
            ss << " :" << reason << "\r\n";
        else
            ss << "\r\n";
        ch->sendTo_all(ss.str());
        ch->remove_Client(ch->GetClientInChannel(user)->getFD());
    }
}