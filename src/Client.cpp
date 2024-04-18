/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:18 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 09:45:29 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Server.hpp"

Client::Client()
{
    Fd = -1;
    IPAddress = "";
    nickname = "";
    username = "";
    registered = false;
    login = false;
    channelsInvite = std::vector<std::string>();
    buffer = "";
    firstMessage = true;
    firstMode = 0;
}

int Client::getFd()
{
    return Fd;
}

void Client::setFd(int fd)
{
    this->Fd = fd;
}

void Client::setIp(std::string IP_address)
{
    this->IPAddress = IP_address;
}

const std::string &Client::getNickname() const
{
    return nickname;
}

void Client::setNickname(const std::string &nickname)
{
    this->nickname = nickname;
}

const std::string &Client::getUsername() const
{
    return username;
}

void Client::setUsername(const std::string &username)
{
    this->username = username;
}

bool Client::isRegistered() const
{
    return registered;
}

void Client::setRegistered(bool registered)
{
    this->registered = registered;
}

const std::vector<std::string> &Client::getChannelsInvite() const
{
    return channelsInvite;
}

void Client::setChannelsInvite(const std::vector<std::string> &channelsInvite)
{
    this->channelsInvite = channelsInvite;
}

const std::string &Client::getBuffer() const
{
    return buffer;
}

void Client::setBuffer(const std::string &buffer)
{
    this->buffer += buffer;
}

void Client::clearBuffer() { buffer.clear(); }

bool Client::isFirstMessage()
{
    return this->firstMessage;
}

void Client::setFirstMessage(bool value)
{
    this->firstMessage = value;
}

std::string Client::getHostname()
{
    return this->getNickname() + "!" + this->getUsername();
}

std::string Client::getIPAddress()
{
    return this->IPAddress;
}

std::string removeAllNewLines0(std::string str)
{
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    return str;
}

bool Client::getChannelInvite(std::string ChName)
{
    for (size_t i = 0; i < this->channelsInvite.size(); i++)
    {
        if (removeAllNewLines0(this->channelsInvite[i]) == removeAllNewLines0(ChName))
            return true;
    }
    return false;
}

void Client::rmChannelInvite(std::string chname)
{
    for (size_t i = 0; i < this->channelsInvite.size(); i++)
    {
        if (removeAllNewLines0(this->channelsInvite[i]) == removeAllNewLines0(chname))
        {
            this->channelsInvite.erase(this->channelsInvite.begin() + i);
            return;
        }
    }
}

void Client::addChannelInvite(std::string chname)
{
    channelsInvite.push_back(chname);
}

int Client::getFirstMode()
{
    return this->firstMode;
}

void Client::setFirstMode(int value)
{
    this->firstMode = value;
}