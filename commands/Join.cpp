#include "../server.hpp"
#include <poll.h>
#include <signal.h>
#include "../client.hpp"

int Server::tokenizationJoin(std::vector<std::pair<std::string, std::string> > &token, std::string cmd, int fd)
{
    std::vector<std::string>    tmp;
    std::string                 channelName, PassChannel, buff;
    std::istringstream          iss(cmd);
    
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
            sendResponse("403 " +  GetClient(fd)->getNickname() + " " +  token[i].first + " :No such channel\r\n", GetClient(fd)->getFD());
            token.erase(token.begin() + i--);
        }
        else
            token[i].first.erase(token[i].first.begin());
    }
    
    // Retourne 1 pour indiquer le succès de la tokenisation
    return 1;
}


// int Server::SearchForClients(std::string nickname)
// {
//     int count = 0;
//     for (size_t i = 0; i < this->channels.size(); i++)
//     {
//         if (this->channels[i].GetClientInChannel(nickname))
//             count++;
//     }
//     return count;
// }

// bool IsInvited(Client *cli, std::string ChName, int flag)
// {
//     if (cli->GetInviteChannel(ChName))
//     {
//         if (flag == 1)
//             cli->RmChannelInvite(ChName);
//         return true;
//     }
//     return false;
// }

// void Server::ExistCh(std::vector<std::pair<std::string, std::string>> &token, int i, int j, int fd)
// {
//     if (this->channels[j].GetClientInChannel(GetClient(fd)->GetNickName())) // if the client is already registered
//         return;
//     if (SearchForClients(GetClient(fd)->GetNickName()) >= 10) // ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
//     {
//         senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n");
//         return;
//     }
//     if (!this->channels[j].GetPassword().empty() && this->channels[j].GetPassword() != token[i].second)
//     { // ERR_BADCHANNELKEY (475) // if the password is incorrect
//         if (!IsInvited(GetClient(fd), token[i].first, 0))
//         {
//             senderror(475, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+k) - bad key\r\n");
//             return;
//         }
//     }
//     if (this->channels[j].GetInvitOnly())
//     { // ERR_INVITEONLYCHAN (473) // if the channel is invit only
//         if (!IsInvited(GetClient(fd), token[i].first, 1))
//         {
//             senderror(473, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+i)\r\n");
//             return;
//         }
//     }
//     if (this->channels[j].GetLimit() && this->channels[j].GetClientsNumber() >= this->channels[j].GetLimit()) // ERR_CHANNELISFULL (471) // if the channel reached the limit of number of clients
//     {
//         senderror(471, GetClient(fd)->GetNickName(), "#" + token[i].first, GetClient(fd)->GetFd(), " :Cannot join channel (+l)\r\n");
//         return;
//     }
//     // add the client to the channel
//     Client *cli = GetClient(fd);
//     this->channels[j].add_client(*cli);
//     if (channels[j].GetTopicName().empty())
//         _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first) +
//                           RPL_NAMREPLY(GetClient(fd)->GetNickName(), channels[j].GetName(), channels[j].clientChannel_list()) +
//                           RPL_ENDOFNAMES(GetClient(fd)->GetNickName(), channels[j].GetName()),
//                       fd);
//     else
//         _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first) +
//                           RPL_TOPICIS(GetClient(fd)->GetNickName(), channels[j].GetName(), channels[j].GetTopicName()) +
//                           RPL_NAMREPLY(GetClient(fd)->GetNickName(), channels[j].GetName(), channels[j].clientChannel_list()) +
//                           RPL_ENDOFNAMES(GetClient(fd)->GetNickName(), channels[j].GetName()),
//                       fd);
//     channels[j].sendTo_all(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), token[i].first), fd);
// }

// void Server::NotExistCh(std::vector<std::pair<std::string, std::string>> &token, int i, int fd)
// {
//     if (SearchForClients(GetClient(fd)->GetNickName()) >= 10) // ERR_TOOMANYCHANNELS (405) // if the client is already in 10 channels
//     {
//         senderror(405, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :You have joined too many channels\r\n");
//         return;
//     }
//     Channel newChannel;
//     newChannel.SetName(token[i].first);
//     newChannel.add_admin(*GetClient(fd));
//     newChannel.set_createiontime();
//     this->channels.push_back(newChannel);
//     // notifiy thet the client joined the channel
//     _sendResponse(RPL_JOINMSG(GetClient(fd)->getHostname(), GetClient(fd)->getIpAdd(), newChannel.GetName()) +
//                       RPL_NAMREPLY(GetClient(fd)->GetNickName(), newChannel.GetName(), newChannel.clientChannel_list()) +
//                       RPL_ENDOFNAMES(GetClient(fd)->GetNickName(), newChannel.GetName()),
//                   fd);
// }

void displayVector(const std::vector<std::pair<std::string, std::string> >& token)
{
    typedef std::vector<std::pair<std::string, std::string> >::const_iterator Iterator;

    for (Iterator it = token.begin(); it != token.end(); ++it)
    {
        std::cout << "First element: " << it->first << ", Second element: " << it->second << std::endl;
    }
}


void Server::JOIN_client(std::string cmd, int fd)
{
    (void)fd;

    std::cout << "JOIN : " << cmd << std::endl;

    std::vector<std::pair<std::string, std::string> > token;


    tokenizationJoin(token, cmd, fd);

    displayVector(token);
    // if (!SplitJoin(token, cmd, fd)) // ERR_NEEDMOREPARAMS (461) // if the channel name is empty
    // {
    //     senderror(461, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Not enough parameters\r\n");
    //     return;
    // }
    // if (token.size() > 10) // ERR_TOOMANYTARGETS (407) // if more than 10 Channels
    // {
    //     senderror(407, GetClient(fd)->GetNickName(), GetClient(fd)->GetFd(), " :Too many channels\r\n");
    //     return;
    // }
    // for (size_t i = 0; i < token.size(); i++)
    // {
    //     bool flag = false;
    //     for (size_t j = 0; j < this->channels.size(); j++)
    //     {
    //         if (this->channels[j].GetName() == token[i].first)
    //         {
    //             ExistCh(token, i, j, fd);
    //             flag = true;
    //             break;
    //         }
    //     }
    //     if (!flag)
    //         NotExistCh(token, i, fd);
    // }
}