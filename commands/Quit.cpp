/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:08 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 22:35:20 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void FindQ(std::string cmd, std::string tofind, std::string &str)
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

std::string SplitQuit(std::string cmd)
{
    std::istringstream stm(cmd);
    std::string reason, str;
    stm >> str;
    FindQ(cmd, str, reason);
    if (reason.empty())
        return std::string("Quit");
    if (reason[0] != ':')
    { // if the message does not start with ':'
        for (size_t i = 0; i < reason.size(); i++)
        {
            if (reason[i] == ' ')
            {
                reason.erase(reason.begin() + i, reason.end());
                break;
            }
        }
        reason.insert(reason.begin(), ':');
    }
    return reason;
}

void Server::RemoveFds(int fd)
{
    for (size_t i = 0; i < this->PollFds.size(); i++)
    {
        if (this->PollFds[i].fd == fd)
        {
            this->PollFds.erase(this->PollFds.begin() + i);
            return;
        }
    }
}

void Server::RemoveClient(int fd)
{
    for (size_t i = 0; i < this->Clients.size(); i++)
    {
        if (this->Clients[i].getFD() == fd)
        {
            this->Clients.erase(this->Clients.begin() + i);
            return;
        }
    }
}

void Server::RmChannels(int fd)
{
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        int flag = 0;
        if (channels[i].get_Client(fd))
        {
            channels[i].remove_Client(fd);
            flag = 1;
        }
        else if (channels[i].get_admin(fd))
        {
            channels[i].remove_admin(fd);
            flag = 1;
        }
        if (channels[i].GetClientsNumber() == 0)
        {
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
        if (flag)
        {
            std::string rpl = ":" + GetClient(fd)->getNickname() + "!~" + GetClient(fd)->getUsername() + "@localhost QUIT Quit\r\n";
            channels[i].sendTo_all(rpl);
        }
    }
}

void Server::QUIT(std::string cmd, int fd)
{
    std::string reason;
    reason = SplitQuit(cmd);
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].get_Client(fd))
        {
            channels[i].remove_Client(fd);
            if (channels[i].GetClientsNumber() == 0)
                channels.erase(channels.begin() + i);
            else
            {
                std::string rpl = ":" + GetClient(fd)->getNickname() + "!~" + GetClient(fd)->getUsername() + "@localhost QUIT " + reason + "\r\n";
                channels[i].sendTo_all(rpl);
            }
        }
        else if (channels[i].get_admin(fd))
        {
            channels[i].remove_admin(fd);
            if (channels[i].GetClientsNumber() == 0)
                channels.erase(channels.begin() + i);
            else
            {
                std::string rpl = ":" + GetClient(fd)->getNickname() + "!~" + GetClient(fd)->getUsername() + "@localhost QUIT " + reason + "\r\n";
                channels[i].sendTo_all(rpl);
            }
        }
    }
    std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
    RmChannels(fd);
    RemoveClient(fd);
    RemoveFds(fd);
    close(fd);
}