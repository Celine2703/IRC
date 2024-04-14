/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:01 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 22:11:17 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void FindPR(std::string cmd, std::string tofind, std::string &str)
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

std::string SplitCmdPR(std::string &cmd, std::vector<std::string> &tmp)
{
    std::stringstream ss(cmd);
    std::string str, reason;
    int count = 2;
    while (ss >> str && count--)
        tmp.push_back(str);
    if (tmp.size() != 2)
        return std::string("");
    FindPR(cmd, tmp[1], reason);
    return reason;
}

// Extrai les noms des canaux à partir d'une chaîne séparée par des virgules.
void Server::ExtractChannels(std::string &str, std::vector<std::string> &channels)
{
    std::string channel;
    for (size_t i = 0; i < str.size(); i++)
    {
        if (str[i] == ',')
        {
            channels.push_back(channel);
            channel.clear();
        }
        else
            channel += str[i];
    }
    if (!channel.empty())
        channels.push_back(channel);
}

// Nettoie et valide les noms des canaux, en supprimant les chaînes vides et les caractères spéciaux.
void Server::CleanChannels(std::vector<std::string> &channels, int fd)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels[i].empty())
        {
            channels.erase(channels.begin() + i--);
        }
        else if (*(channels[i].begin()) == '#')
        {
            channels[i].erase(channels[i].begin());
        }
        else
        {
            senderror(403, GetClient(fd)->getNickname(), channels[i], GetClient(fd)->getFD(), " :No such channel\r\n");
            channels.erase(channels.begin() + i--);
        }
    }
}

// Normalise la raison en supprimant le préfixe ':' ou en limitant la chaîne au premier espace.
void Server::NormalizeReason(std::string &reason)
{
    if (reason[0] == ':')
        reason.erase(reason.begin());
    else
    {
        size_t pos = reason.find(' ');
        if (pos != std::string::npos)
            reason = reason.substr(0, pos);
    }
}

// Méthode principale qui utilise les sous-méthodes pour traiter la commande.
int Server::SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd)
{
    reason = SplitCmdPR(cmd, tmp);
    if (tmp.size() < 2)
    {
        tmp.clear();
        return 0;
    }
    tmp.erase(tmp.begin());
    std::string str = tmp[0];
    tmp.clear();

    ExtractChannels(str, tmp);
    CleanChannels(tmp, fd);
    NormalizeReason(reason);

    return 1;
}


void Server::PART(std::string cmd, int fd)
{
    std::vector<std::string> tmp;
    std::string reason;
    if (!SplitCmdPart(cmd, tmp, reason, fd)) // ERR_NEEDMOREPARAMS (461) // if the channel name is empty
    {
        senderror(461, GetClient(fd)->getNickname(), GetClient(fd)->getFD(), " :Not enough parameters\r\n");
        return;
    }
    for (size_t i = 0; i < tmp.size(); i++)
    {
        bool flag = false;
        for (size_t j = 0; j < this->channels.size(); j++)
        { // search for the channel
            if (this->channels[j].GetName() == tmp[i])
            { // check if the channel exist
                flag = true;
                if (!channels[j].get_Client(fd) && !channels[j].get_admin(fd)) // ERR_NOTONCHANNEL (442) // if the client is not in the channel
                {
                    senderror(442, GetClient(fd)->getNickname(), "#" + tmp[i], GetClient(fd)->getFD(), " :You're not on that channel\r\n");
                    continue;
                }
                std::stringstream ss;
                ss << ":" << GetClient(fd)->getNickname() << "!~" << GetClient(fd)->getUsername() << "@"
                   << "localhost"
                   << " PART #" << tmp[i];
                if (!reason.empty())
                    ss << " :" << reason << "\r\n";
                else
                    ss << "\r\n";
                channels[j].sendTo_all(ss.str());
                if (channels[j].get_admin(channels[j].GetClientInChannel(GetClient(fd)->getNickname())->getFD()))
                    channels[j].remove_admin(channels[j].GetClientInChannel(GetClient(fd)->getNickname())->getFD());
                else
                    channels[j].remove_Client(channels[j].GetClientInChannel(GetClient(fd)->getNickname())->getFD());
                if (channels[j].GetClientsNumber() == 0)
                    channels.erase(channels.begin() + j);
            }
        }
        if (!flag) // ERR_NOSUCHCHANNEL (403) // if the channel doesn't exist
            senderror(403, GetClient(fd)->getNickname(), "#" + tmp[i], GetClient(fd)->getFD(), " :No such channel\r\n");
    }
}