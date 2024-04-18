/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:12:32 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 09:36:47 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void prin(std::string cmd)
{
    std::cout << cmd << std::endl;
}

void printVector2(const std::vector<std::string> &vec)
{
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

bool Server::checkParameters(std::vector<std::string> &scmd, int &fd)
{
    if (scmd.size() < 3)
    {
        senderror(461, findClientByFd(fd)->getNickname(), fd, " :Not enough parameters\n");
        return false;
    }

    prin("\nAVANT channelname");
    printVector2(scmd);
    std::string channelname = scmd[2].substr(1);
    prin("\nAPRES channelname");
    prin(channelname);
    if (scmd[2][0] != '#' || !findChannelByName(channelname))
    {
        senderror(403, channelname, fd, " :No such channel\n");
        return false;
    }

    prin("\nreturn tue checkParameters");
    return true;
}

bool Server::checkChannelMembershipAndRights(std::vector<std::string> &scmd, int &fd)
{
    std::string channelname = scmd[2].substr(1);
    prin("\n checkChannelMembershipAndRights AVANT ");
    prin(channelname);
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
    printVector2(scmd);
    if (!checkParameters(scmd, fd))
    {
        prin("failed checkParameters");
        return;
    }
    prin("after check parameter");
    printVector2(scmd);
    if (!checkChannelMembershipAndRights(scmd, fd))
    {
        prin("failed checkChannelMembershipAndRights");
        return;
    }
    prin("after checkChannelMembershipAndRights");
    printVector2(scmd);
    if (!handleChannelLimitsAndInvitations(scmd, fd))
    {
        prin("failed handleChannelLimitsAndInvitations");
        return;
    }
    prin("after handleChannelLimitsAndInvitations");
    printVector2(scmd);
    Client *clt = findClientByNick(scmd[1]);
    if (!clt)
    {
        senderror(401, scmd[1], fd, " :No such nick\n");
        return;
    }

    prin("normalement c'est bon");
    printVector2(scmd);
    clt->addChannelInvite(scmd[2].substr(1));
    std::string rep1 = ": 341 " + findClientByFd(fd)->getNickname() + " " + clt->getNickname() + " " + scmd[2] + "\n";
    sendResponse(rep1, fd);
    std::string rep2 = ":" + clt->getHostname() + " INVITE " + clt->getNickname() + " " + scmd[2] + "\n";
    sendResponse(rep2, clt->getFd());
}
