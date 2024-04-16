/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 20:25:45 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 21:51:02 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

// Trouve et extrait la sous-chaîne après un mot spécifié dans une commande.
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

// Divise la commande en mots et trouve la raison associée à un mot clé.
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

// Extrait la raison d'un argument de commande, en nettoyant les préfixes et en limitant à l'espace.
std::string Server::extractReason(std::string &reason)
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

// Nettoie les noms de canaux dans une liste, supprimant les canaux invalides ou vides.
void Server::cleanAllChannelNames(std::vector<std::string> &tmp, int fd)
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
            senderror(403, findClientByFd(fd)->getNickname(), tmp[i], findClientByFd(fd)->getFd(), " :No such channel\n");
            tmp.erase(tmp.begin() + i--);
        }
    }
}

// Sépare les noms de canaux basés sur des virgules dans une chaîne donnée.
void Server::splitChannelNames(const std::string &str, std::vector<std::string> &tmp)
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

// Traite la commande Kick, séparant les arguments et nettoyant le nom du canal.
std::string Server::tokenizationKickCommand(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd)
{
    std::string reason = SplitCmdK(cmd, tmp);

    if (tmp.size() < 3)
        return std::string("");

    tmp.erase(tmp.begin());
    std::string str = tmp[0];
    user = tmp[1];
    tmp.clear();

    splitChannelNames(str, tmp);
    cleanAllChannelNames(tmp, fd);
    reason = extractReason(reason);

    return reason;
}

// Applique la commande KICK à l'utilisateur spécifié dans les canaux spécifiés.
void Server::KICK(std::string cmd, int fd)
{
    std::vector<std::string> tmp;
    std::string reason, user;
    reason = tokenizationKickCommand(cmd, tmp, user, fd);

    if (!checkParameters(user, fd))
        return;

    kickOutChannel(user, reason, tmp, fd);
}

// Vérifie si les paramètres nécessaires sont présents pour une commande.
bool Server::checkParameters(const std::string &user, int fd)
{
    if (user.empty())
    {
        senderror(461, "", fd, " :Not enough parameters\n");
        return false;
    }
    return true;
}

// Gère l'action de kick pour chaque canal impliqué, envoyant les notifications nécessaires.
void Server::kickOutChannel(const std::string &user, const std::string &reason, std::vector<std::string> &tmp, int fd)
{
    for (size_t i = 0; i < tmp.size(); i++)
    {
        Channel *ch = findChannelByName(tmp[i]);
        if (!ch)
        {
            senderror(403, "#" + tmp[i], fd, " :No such channel\n");
            continue;
        }
        if (!ch->getClientInChannelByFd(fd) && !ch->getAdmin(fd))
        {
            senderror(442, "#" + tmp[i], fd, " :You're not on that channel\n");
            continue;
        }
        if (!ch->getAdmin(fd))
        {
            senderror(482, "#" + tmp[i], fd, " :You're not channel operator\n");
            continue;
        }
        if (!ch->findClientByFdInChannel(user))
        {
            senderror(441, "#" + tmp[i], fd, " :They aren't on that channel\n");
            continue;
        }

        std::stringstream ss;
        ss << ":" << findClientByFd(fd)->getNickname() << "!~" << findClientByFd(fd)->getUsername() << "@localhost KICK #" << tmp[i] << " " << user;
        if (!reason.empty())
            ss << " :" << reason << "\n";
        else
            ss << "\n";
        ch->sendToAll(ss.str());
        ch->removeClient(ch->findClientByFdInChannel(user)->getFd());
    }
}