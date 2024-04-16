/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 18:39:22 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 19:56:02 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// Renvoie l'heure actuelle sous forme de chaîne de caractères
std::string Server::tTopic()
{
    std::time_t current = std::time(NULL);
    std::stringstream res;

    res << current;
    return res.str();
}

// Renvoie le sujet d'un canal
std::string Server::gettopic(std::string &input)
{
    size_t pos = input.find(":");
    if (pos == std::string::npos)
    {
        return "";
    }
    return input.substr(pos);
}

// Renvoie la position de deux points dans une commande
int Server::getpos(std::string &cmd)
{
    for (int i = 0; i < (int)cmd.size(); i++)
        if (cmd[i] == ':' && (cmd[i - 1] == 32))
            return i;
    return -1;
}

// Vérifie si une commande a suffisamment de paramètres
bool Server::hasSufficientParameters(const std::vector<std::string> &scmd, int fd)
{
    if (scmd.size() < 2)
    {
        senderror(461, GetClient(fd)->getNickname(), fd, " :Not enough parameters\r\n");
        return false;
    }
    return true;
}

// Valide l'existence d'un canal
Channel *Server::validateChannel(const std::string &channelName, int fd)
{
    Channel *channel = GetChannel(channelName);
    if (!channel)
    {
        senderror(403, "#" + channelName, fd, " :No such channel\r\n");
        return NULL;
    }
    if (!channel->get_Client(fd) && !channel->get_admin(fd))
    {
        senderror(442, "#" + channelName, fd, " :You're not on that channel\r\n");
        return NULL;
    }
    return channel;
}

// Gère l'affichage du sujet du canal
void Server::handleTopicDisplay(Channel *channel, const std::string &channelName, int fd)
{
    if (channel->GetTopicName().empty())
    {
        sendResponse(": 331 " + GetClient(fd)->getNickname() + " #" + channelName + " :No topic is set\r\n", fd);
    }
    else
    {
        sendResponse(": 332 " + GetClient(fd)->getNickname() + " #" + channelName + " " + channel->GetTopicName() + "\r\n", fd);
        sendResponse(": 333 " + GetClient(fd)->getNickname() + " #" + channelName + " " + GetClient(fd)->getNickname() + " " + channel->GetTime() + "\r\n", fd);
    }
}

// Met à jour le sujet du canal
void Server::updateTopic(Channel *channel, const std::string &channelName, const std::string &topic, int fd)
{
    if (channel->Gettopic_restriction() && !channel->get_admin(fd))
    {
        senderror(482, "#" + channelName, fd, " :You're not a channel operator\r\n");
    }
    else
    {
        channel->SetTopicName(topic);
        channel->SetTime(tTopic());
        std::string rpl = ":" + GetClient(fd)->getNickname() + "!" + GetClient(fd)->getUsername() + "@localhost TOPIC #" + channelName + " :" + topic + "\r\n";
        channel->sendTo_all(rpl);
    }
}

// Traite la commande TOPIC
void Server::TOPIC(std::string &cmd, int &fd)
{
    std::vector<std::string> scmd = tokenizationCommand(cmd);
    if (!hasSufficientParameters(scmd, fd))
        return;

    std::string channelName = scmd[1].substr(1);
    Channel *channel = validateChannel(channelName, fd);
    if (!channel)
        return;

    if (scmd.size() == 2)
    {
        handleTopicDisplay(channel, channelName, fd);
    }
    else
    {
        std::string topic = cmd.substr(cmd.find_first_of(':') + 1);
        updateTopic(channel, channelName, topic, fd);
    }
}
