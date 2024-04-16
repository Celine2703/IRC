/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:56 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 19:36:51 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <poll.h>
#include <signal.h>
#include "../includes/Client.hpp"

// Fonction pour tokeniser une commande JOIN et extraire le nom de canal et le mot de passe
int Server::tokenizationJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
    std::vector<std::string> tmp;
    std::string channelName, PassChannel, buff;
    std::istringstream iss(cmd);

    // Extraction des tokens de la commande et stockage dans le vecteur temporaire
    while (iss >> cmd)
        tmp.push_back(cmd);

    // Vérification de la taille des tokens et nettoyage du vecteur de tokens si nécessaire
    if (tmp.size() < 2)
    {
        token.clear();
        return 0;
    }

    // Traitement des tokens pour extraire le nom de canal et le mot de passe
    tmp.erase(tmp.begin());
    channelName = tmp[0];
    tmp.erase(tmp.begin());
    if (!tmp.empty())
    {
        PassChannel = tmp[0];
        tmp.clear();
    }

    // Tokenisation du nom de canal
    for (size_t i = 0; i < channelName.size(); i++)
    {
        if (channelName[i] == ',')
        {
            token.push_back(std::make_pair(buff, ""));
            buff.clear();
        }
        else
            buff += channelName[i];
    }
    token.push_back(std::make_pair(buff, ""));

    // Tokenisation du mot de passe s'il existe
    if (!PassChannel.empty())
    {
        size_t j = 0;
        buff.clear();
        for (size_t i = 0; i < PassChannel.size(); i++)
        {
            if (PassChannel[i] == ',')
            {
                token[j].second = buff;
                j++;
                buff.clear();
            }
            else
                buff += PassChannel[i];
        }
        token[j].second = buff;
    }

    // Suppression des noms de canal vides du vecteur de tokens
    for (size_t i = 0; i < token.size(); i++)
    {
        if (token[i].first.empty())
            token.erase(token.begin() + i--);
    }

    // Vérification et traitement des noms de canal invalides
    for (size_t i = 0; i < token.size(); i++)
    {
        if (*(token[i].first.begin()) != '#')
        {
            // Envoi d'une réponse d'erreur si le nom de canal est invalide
            // il faut créer un channel quand ce dernier nexiste oas
            sendResponse("403 " + GetClient(fd)->getNickname() + " " + token[i].first + " :No such channel\r\n", GetClient(fd)->getFD());
            token.erase(token.begin() + i--);
        }
        else
            token[i].first.erase(token[i].first.begin());
    }

    // Retourne 1 pour indiquer le succès de la tokenisation
    return 1;
}

// Fonction pour vérifier si un Client est invité dans un canal
bool IsInvited(Client *cli, std::string ChName, int flag)
{
    if (cli->getChannelInvite(ChName))
    {
        if (flag == 1)
            cli->rmChannelInvite(ChName);
        return true;
    }
    return false;
}

// Fonction pour traiter le cas où un canal existe déjà
void Server::ExistCh(std::vector<std::pair<std::string, std::string> > &token, int i, int j, int fd)
{
    if (this->channels[j].GetClientInChannel(GetClient(fd)->getNickname()))
        return;
    if (SearchForClients(GetClient(fd)->getNickname()) >= 10)
    {
        senderror(405, GetClient(fd)->getNickname(), GetClient(fd)->getFD(), " :You have joined too many channels\r\n");
        return;
    }
    if (!this->channels[j].GetPassword().empty() && this->channels[j].GetPassword() != token[i].second)
    {
        if (!IsInvited(GetClient(fd), token[i].first, 0))
        {
            senderror(475, GetClient(fd)->getNickname(), "#" + token[i].first, GetClient(fd)->getFD(), " :Cannot join channel (+k) - bad key\r\n");
            return;
        }
    }
    if (this->channels[j].GetInvitOnly())
    {
        if (!IsInvited(GetClient(fd), token[i].first, 1))
        {
            senderror(473, GetClient(fd)->getNickname(), "#" + token[i].first, GetClient(fd)->getFD(), " :Cannot join channel (+i)\r\n");
            return;
        }
    }
    if (this->channels[j].GetLimit() && this->channels[j].GetClientsNumber() >= this->channels[j].GetLimit())
    {
        senderror(471, GetClient(fd)->getNickname(), "#" + token[i].first, GetClient(fd)->getFD(), " :Cannot join channel (+l)\r\n");
        return;
    }

    Client *cli = GetClient(fd);
    this->channels[j].add_Client(*cli);
    if (channels[j].GetTopicName().empty())
        sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIPAddress(), token[i].first) +
                         RPL_NAMREPLY(GetClient(fd)->getNickname(), channels[j].GetName(), channels[j].ClientChannel_list()) +
                         RPL_ENDOFNAMES(GetClient(fd)->getNickname(), channels[j].GetName()),
                     fd);
    else
        sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIPAddress(), token[i].first) +
                         RPL_TOPICIS(GetClient(fd)->getNickname(), channels[j].GetName(), channels[j].GetTopicName()) +
                         RPL_NAMREPLY(GetClient(fd)->getNickname(), channels[j].GetName(), channels[j].ClientChannel_list()) +
                         RPL_ENDOFNAMES(GetClient(fd)->getNickname(), channels[j].GetName()),
                     fd);
    channels[j].sendTo_all(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIPAddress(), token[i].first), fd);
}

// Fonction pour traiter le cas où un canal n'existe pas encore
void Server::NotExistCh(std::vector<std::pair<std::string, std::string> > &token, int i, int fd)
{
    if (SearchForClients(GetClient(fd)->getNickname()) >= 10)
    {
        senderror(405, GetClient(fd)->getNickname(), GetClient(fd)->getFD(), " :You have joined too many channels\r\n");
        return;
    }
    Channel newChannel;
    newChannel.SetName(removeFirstBackLine(token[i].first));
    newChannel.add_admin(*GetClient(fd));
    newChannel.set_createiontime();
    this->channels.push_back(newChannel);
    // notifiy thet the Client joined the channel
    sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIPAddress(), newChannel.GetName()) +
                     RPL_NAMREPLY(GetClient(fd)->getNickname(), newChannel.GetName(), newChannel.ClientChannel_list()) +
                     RPL_ENDOFNAMES(GetClient(fd)->getNickname(), newChannel.GetName()),
                 fd);
}

// Fonction pour gérer la commande JOIN d'un Client
void Server::JOIN_Client(std::string cmd, int fd)
{
    (void)fd;

    std::cout << "JOIN : " << cmd << std::endl;

    std::vector<std::pair<std::string, std::string> > token;

    // pas de nom de channel
    if (!tokenizationJoin(token, cmd, fd))
    {
        senderror(461, GetClient(fd)->getNickname(), GetClient(fd)->getFD(), " :Not enough parameters\r\n");
        return;
    }

    // le Client a deja trop de channel
    if (token.size() > 10)
    {
        senderror(407, GetClient(fd)->getNickname(), GetClient(fd)->getFD(), " :Too many channels\r\n");
        return;
    }

    for (size_t i = 0; i < token.size(); i++)
    {
        bool flag = false;
        for (size_t j = 0; j < this->channels.size(); j++)
        {
            if (this->channels[j].GetName() == token[i].first)
            {
                ExistCh(token, i, j, fd);
                flag = true;
                break;
            }
        }
        if (!flag)
            NotExistCh(token, i, fd);
    }
}