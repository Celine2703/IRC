/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:12:32 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 21:51:02 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"


bool Server::checkParameters(std::vector<std::string> &scmd, int &fd)
{
    if (scmd.size() < 3)
    {
        senderror(461, findClientByFd(fd)->getNickname(), fd, " :Not enough parameters\n");
        return false;
    }

    std::string channelname = scmd[2].substr(1);
    if (scmd[2][0] != '#' || !findChannelByName(channelname))
    {
        senderror(403, channelname, fd, " :No such channel\n");
        return false;
    }

    return true;
}

bool Server::checkChannelMembershipAndRights(std::vector<std::string> &scmd, int &fd)
{
    std::string channelname = scmd[2].substr(1);
    if (!(findChannelByName(channelname)->getClientInChannelByFd(fd)) && !(findChannelByName(channelname)->getAdmin(fd)))
    {
        senderror(442, channelname, fd, " :You're not on that channel\n");
        return false;
    }

    if (findChannelByName(channelname)->GetInvitOnly() && !findChannelByName(channelname)->getAdmin(fd))
    {
        senderror(482, findChannelByName(channelname)->getClientInChannelByFd(fd)->getNickname(), scmd[1], fd, " :You're not channel operator\n");
        return false;
    }

    return true;
}

bool Server::handleChannelLimitsAndInvitations(std::vector<std::string> &scmd, int &fd)
{
    std::string channelname = scmd[2].substr(1);
    if (findChannelByName(channelname)->findClientByFdInChannel(scmd[1]))
    {
        senderror(443, findClientByFd(fd)->getNickname(), channelname, fd, " :is already on channel\n");
        return false;
    }

    if (findChannelByName(channelname)->GetLimit() && findChannelByName(channelname)->findClientByFdsNumber() >= findChannelByName(channelname)->GetLimit())
    {
        senderror(473, findChannelByName(channelname)->getClientInChannelByFd(fd)->getNickname(), channelname, fd, " :Cannot invit to channel (+i)\n");
        return false;
    }

    return true;
}

void Server::INVITE(std::string &cmd, int &fd)
{
    std::vector<std::string> scmd = tokenizationCommand(cmd);
    if (!checkParameters(scmd, fd))
        return;
    if (!checkChannelMembershipAndRights(scmd, fd))
        return;
    if (!handleChannelLimitsAndInvitations(scmd, fd))
        return;

    Client *clt = findClientByNick(scmd[1]);
    if (!clt)
    {
        senderror(401, scmd[1], fd, " :No such nick\n");
        return;
    }

    clt->addChannelInvite(scmd[2].substr(1));
    std::string rep1 = ": 341 " + findClientByFd(fd)->getNickname() + " " + clt->getNickname() + " " + scmd[2] + "\n";
    sendResponse(rep1, fd);
    std::string rep2 = ":" + clt->getHostname() + " INVITE " + clt->getNickname() + " " + scmd[2] + "\n";
    sendResponse(rep2, clt->getFd());
}
