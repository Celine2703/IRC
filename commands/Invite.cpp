/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:12:32 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 19:20:47 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::INVITE(std::string &cmd, int &fd)
{
    std::vector<std::string> scmd = tokenizationCommand(cmd);
    
    if (scmd.size() < 3)
    {
        senderror(461, GetClient(fd)->getNickname(), fd, " :Not enough parameters\r\n");
        return;
    }
    std::string channelname = scmd[2].substr(1);
    if (scmd[2][0] != '#' || !GetChannel(channelname))
    {
        senderror(403, channelname, fd, " :No such channel\r\n");
        return;
    }
    if (!(GetChannel(channelname)->get_Client(fd)) && !(GetChannel(channelname)->get_admin(fd)))
    {
        senderror(442, channelname, fd, " :You're not on that channel\r\n");
        return;
    }
    if (GetChannel(channelname)->GetClientInChannel(scmd[1]))
    {
        senderror(443, GetClient(fd)->getNickname(), channelname, fd, " :is already on channel\r\n");
        return;
    }
    Client *clt = GetClientNick(scmd[1]);
    if (!clt)
    {
        senderror(401, scmd[1], fd, " :No such nick\r\n");
        return;
    }
    if (GetChannel(channelname)->GetInvitOnly() && !GetChannel(channelname)->get_admin(fd))
    {
        senderror(482, GetChannel(channelname)->get_Client(fd)->getNickname(), scmd[1], fd, " :You're not channel operator\r\n");
        return;
    }
    if (GetChannel(channelname)->GetLimit() && GetChannel(channelname)->GetClientsNumber() >= GetChannel(channelname)->GetLimit())
    {
        senderror(473, GetChannel(channelname)->get_Client(fd)->getNickname(), channelname, fd, " :Cannot invit to channel (+i)\r\n");
        return;
    }
    clt->addChannelInvite(channelname);
    std::string rep1 = ": 341 " + GetClient(fd)->getNickname() + " " + clt->getNickname() + " " + scmd[2] + "\r\n";
    sendResponse(rep1, fd);
    std::string rep2 = ":" + clt->getHostname() + " INVITE " + clt->getNickname() + " " + scmd[2] + "\r\n";
    sendResponse(rep2, clt->getFD());
}