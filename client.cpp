#include "client.hpp"

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
}

int Client::getFD()
{
    return Fd;
}

void Client::setFD(int fd)
{
    this->Fd = fd;
}

void Client::setIP(std::string IP_address)
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


bool	Client::isFirstMessage()
{
	return this->firstMessage;
}

void	Client::setFirstMessage(bool value)
{
    this->firstMessage = value;
}