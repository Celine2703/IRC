/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:52:08 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 21:51:02 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// Recherche une commande spécifique et extrait la chaîne suivante
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

// Divise une commande et retourne la raison formatée pour quitter
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

// Supprime un file descriptor de la liste de polling
void Server::removeFd(int fd)
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

// Supprime un client de la liste des clients
void Server::removeClient(int fd)
{
    for (size_t i = 0; i < this->Clients.size(); i++)
    {
        if (this->Clients[i].getFd() == fd)
        {
            this->Clients.erase(this->Clients.begin() + i);
            return;
        }
    }
}

// Supprime un client des canaux et gère les notifications
void Server::rmChannels(int fd)
{
    for (size_t i = 0; i < this->channels.size(); i++)
    {
        int flag = 0;
        if (channels[i].getClientInChannelByFd(fd))
        {
            channels[i].removeClient(fd);
            flag = 1;
        }
        else if (channels[i].getAdmin(fd))
        {
            channels[i].removeAdmin(fd);
            flag = 1;
        }
        if (channels[i].findClientByFdsNumber() == 0)
        {
            channels.erase(channels.begin() + i);
            i--;
            continue;
        }
        if (flag)
        {
            std::string rpl = ":" + findClientByFd(fd)->getNickname() + "!~" + findClientByFd(fd)->getUsername() + "@localhost QUIT Quit\n";
            channels[i].sendToAll(rpl);
        }
    }
}

// Extraire la raison du QUIT à partir de la commande.
std::string Server::extractQuitReason(std::string cmd)
{
    return SplitQuit(cmd);
}

// Gérer la suppression du client des canaux et l'envoi des notifications.
void Server::removeClientChannel(int fd, const std::string &reason)
{
    for (size_t i = 0; i < channels.size();)
    {
        bool isClient = channels[i].getClientInChannelByFd(fd);
        bool isAdmin = channels[i].getAdmin(fd);

        if (isClient || isAdmin)
        {
            channels[i].removeClient(fd);
            if (channels[i].findClientByFdsNumber() == 0)
            {
                channels.erase(channels.begin() + i);
            }
            else
            {
                std::string rpl = ":" + findClientByFd(fd)->getNickname() + "!~" + findClientByFd(fd)->getUsername() + "@localhost QUIT " + reason + "\n";
                channels[i].sendToAll(rpl);
                i++;
            }
        }
        else
        {
            i++;
        }
    }
}

// Afficher une notification de déconnexion du client sur le serveur.
void Server::showDisconnectionClient(int fd)
{
    std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
}

// Nettoyer toutes les ressources associées au client.
void Server::freeClient(int fd)
{
    rmChannels(fd);
    removeClient(fd);
    removeFd(fd);
    close(fd);
}

// Méthode principale qui utilise les sous-méthodes pour traiter la commande QUIT.
void Server::QUIT(std::string cmd, int fd)
{
    std::string reason = extractQuitReason(cmd);
    removeClientChannel(fd, reason);
    showDisconnectionClient(fd);
    freeClient(fd);
}