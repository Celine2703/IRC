/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 20:45:46 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 08:53:19 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void printVector(const std::vector<std::string> &vec)
{
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

std::string Server::modeToAppend(std::string chain, char opera, char mode)
{
    std::stringstream ss;

    ss.clear();
    char last = '\0';
    for (size_t i = 0; i < chain.size(); i++)
    {
        if (chain[i] == '+' || chain[i] == '-')
            last = chain[i];
    }
    if (last != opera)
        ss << opera << mode;
    else
        ss << mode;
    return ss.str();
}

void Server::getCmdArgs(std::string cmd, std::string &name, std::string &modeset, std::string &params)
{
    std::istringstream stm(cmd);
    stm >> name;
    stm >> modeset;
    size_t found = cmd.find_first_not_of(name + modeset + " \t\v");
    if (found != std::string::npos)
        params = cmd.substr(found);
}

std::vector<std::string> Server::splitParams(std::string params)
{
    std::vector<std::string> tokens;
    std::string param;

    if (!params.empty() && params[0] == ':')
    {
        params.erase(0, 1);
    }

    std::istringstream stm(params);
    while (std::getline(stm, param, ','))
    {
        tokens.push_back(param);
        param.clear();
    }
    return tokens;
}

void    print(std::string cmd)
{
    std::cout << cmd << std::endl;
}

std::vector<std::string> removeFirstTwoAndReturnNew(std::vector<std::string> vec)
{
    std::vector<std::string> newVec;
    if (vec.size() >= 3)
        newVec.insert(newVec.end(), vec.begin() + 2, vec.end());
    else
        return vec;
    return newVec;
}

void Server::MODE(std::string &cmd, int fd)
{
    std::string channelName;
    std::string params;
    std::string modeset;
    std::stringstream mode_chain;
    std::string arguments = ":";
    Channel *channel;
    char opera = '\0';

    arguments.clear();
    mode_chain.clear();
    Client *cli = findClientByFd(fd);
    size_t found = cmd.find_first_not_of("MODEmode \t\v");

    print("AVANT CMD");
    print(cmd);
    
    if (found != std::string::npos)
        cmd = cmd.substr(found - 1);
    else
    {
        sendResponse(ERR_NOTENOUGHPARAM(cli->getNickname()), fd);
        return;
    }

    print("APRES CMD");
    print(cmd);

    

    getCmdArgs(cmd, channelName, modeset, params);

    print("APRES CMD ARGS params");
    print(params);

    std::vector<std::string> tokens = removeFirstTwoAndReturnNew(tokenizationCommand(cmd));

    print("vector tokens");
    printVector(tokens);

    print("\nvector my fonction");

    printVector(tokenizationCommand(cmd));

    print("\n");

    
    if (channelName[0] != '#' || !(channel = findChannelByName(channelName.substr(1))))
    {
        sendResponse(ERR_CHANNELNOTFOUND(cli->getUsername(), channelName), fd);
        return;
    }
    else if (!channel->getClientInChannelByFd(fd) && !channel->getAdmin(fd))
    {
        senderror(442, findClientByFd(fd)->getNickname(), channelName, findClientByFd(fd)->getFd(), " :You're not on that channel\r\n");
        return;
    }
    else if (modeset.empty()) // response with the channel modes (MODE #channel)
    {
        sendResponse(RPL_CHANNELMODES(cli->getNickname(), channel->GetName(), channel->getModes()) +
                         RPL_CREATIONTIME(cli->getNickname(), channel->GetName(), channel->get_creationtime()),
                     fd);
        return;
    }
    else if (!channel->getAdmin(fd)) // client is not channel operator
    {
        sendResponse(ERR_NOTOPERATOR(channel->GetName()), fd);
        return;
    }
    else if (channel)
    {
        size_t pos = 0;
        for (size_t i = 0; i < modeset.size(); i++)
        {
            if (modeset[i] == '+' || modeset[i] == '-')
                opera = modeset[i];
            else
            {
                if (modeset[i] == 'i') // invite mode
                    mode_chain << inviteOnly(channel, opera, mode_chain.str());
                else if (modeset[i] == 't') // topic restriction mode
                    mode_chain << topicRestriction(channel, opera, mode_chain.str());
                else if (modeset[i] == 'k') // password set/remove
                    mode_chain << passwordMode(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
                else if (modeset[i] == 'o') // set/remove user operator privilege
                    mode_chain << operator_privilege(tokens, channel, pos, fd, opera, mode_chain.str(), arguments);
                else if (modeset[i] == 'l') // set/remove channel limits
                    mode_chain << channel_limit(tokens, channel, pos, opera, fd, mode_chain.str(), arguments);
                else
                    sendResponse(ERR_UNKNOWNMODE(cli->getNickname(), channel->GetName(), modeset[i]), fd);
            }
        }
    }
    std::string chain = mode_chain.str();
    std::cout << "chain " << chain << std::endl;
    if (chain.empty())
        return;
    channel->sendToAll(RPL_CHANGEMODE(cli->getHostname(), channel->GetName(), mode_chain.str(), arguments));
}

std::string Server::inviteOnly(Channel *channel, char opera, std::string chain)
{
    std::string param;
    param.clear();
    if (opera == '+' && !channel->getModeAtindex(0))
    {
        channel->setModeAtindex(0, true);
        channel->setInvitOnly(1);
        param = modeToAppend(chain, opera, 'i');
    }
    else if (opera == '-' && channel->getModeAtindex(0))
    {
        channel->setModeAtindex(0, false);
        channel->setInvitOnly(0);
        param = modeToAppend(chain, opera, 'i');
    }
    return param;
}

std::string Server::topicRestriction(Channel *channel, char opera, std::string chain)
{
    std::string param;
    param.clear();
    if (opera == '+' && !channel->getModeAtindex(1))
    {
        channel->setModeAtindex(1, true);
        channel->setTopicRestriction(true);
        param = modeToAppend(chain, opera, 't');
    }
    else if (opera == '-' && channel->getModeAtindex(1))
    {
        channel->setModeAtindex(1, false);
        channel->setTopicRestriction(false);
        param = modeToAppend(chain, opera, 't');
    }
    return param;
}

std::string removeAllNewLines(std::string str)
{
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    return str;
}

bool validPassword(std::string password)
{
    std::string passwordWithOutSpace = removeAllNewLines(password);
    if (passwordWithOutSpace.empty())
        return false;
    for (size_t i = 0; i < passwordWithOutSpace.size(); i++)
    {
        if (!std::isalnum(passwordWithOutSpace[i]) && passwordWithOutSpace[i] != '_')
        {
            std::cout << "le false " << passwordWithOutSpace[i] << std::endl;
            return false;
        }
    }
    return true;
}

std::string Server::passwordMode(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments)
{
    std::string param;
    std::string pass;

    param.clear();
    pass.clear();

    if (tokens.size() > pos)
    {
        pass = tokens[pos++];
    }
    else
    {
        sendResponse(ERR_NEEDMODEPARM(channel->GetName(), std::string("(k)")), fd);
        return param;
    }

    printVector(tokens);
    std::cout << "pass " << pass << std::endl;
    if (!validPassword(pass))
    {
        sendResponse(ERR_INVALIDMODEPARM(channel->GetName(), std::string("(k)")), fd);
        return param;
    }
    if (opera == '+')
    {
        channel->setModeAtindex(2, true);
        channel->SetPassword(pass);
        if (!arguments.empty())
            arguments += " ";
        arguments += pass;
        param = modeToAppend(chain, opera, 'k');
    }
    else if (opera == '-' && channel->getModeAtindex(2))
    {
        if (removeAllNewLines(pass) == removeAllNewLines(channel->GetPassword()))
        {
            channel->setModeAtindex(2, false);
            channel->SetPassword("");
            param = modeToAppend(chain, opera, 'k');
        }
        else
            sendResponse(ERR_KEYSET(channel->GetName()), fd);
    }
    return param;
}

std::string Server::operator_privilege(std::vector<std::string> tokens, Channel *channel, size_t &pos, int fd, char opera, std::string chain, std::string &arguments)
{
    std::string user;
    std::string param;

    param.clear();
    user.clear();
    if (tokens.size() > pos)
    {
        user = tokens[pos];
        std::cout << "1 user " << user << std::endl;
        std::cout << "1 all vector " << std::endl;
        printVector(tokens);
    }
    else
    {
        sendResponse(ERR_NEEDMODEPARM(channel->GetName(), "(o)"), fd);
        return param;
    }
    std::string userWithOutSpaces = removeAllNewLines(user);
    std::cout << "1 userWithOutSpaces " << userWithOutSpaces << std::endl;

    if (!channel->ClientInChannel(userWithOutSpaces))
    {
        sendResponse(ERR_NOSUCHNICK(channel->GetName(), user), fd);
        return param;
    }
    if (opera == '+')
    {
        std::cout << "+" << std::endl;
        std::cout << "chain avant " << chain << std::endl;
        std::cout << "user avant " << user << std::endl;
        channel->setModeAtindex(3, true);
        if (channel->changeClientToAdmin(user))
        {
            std::cout << "changeClientToAdmin oui" << std::endl;
            param = modeToAppend(chain, opera, 'o');
            if (!arguments.empty())
                arguments += " ";
            arguments += user;
        }
        std::cout << "chain apres " << chain << std::endl;
    }
    else if (opera == '-')
    {
        std::cout << "-" << std::endl;
        channel->setModeAtindex(3, false);
        if (channel->changeAdminToClient(user))
        {
            param = modeToAppend(chain, opera, 'o');
            if (!arguments.empty())
                arguments += " ";
            arguments += user;
        }
    }
    std::cout << "param " << param << std::endl;
    return param;
}

bool Server::isvalidLimit(std::string &limit)
{
    std::cout << "limit " << limit << std::endl;
    std::string limitWithOutSpaces = removeAllNewLines(limit);
    return (!(limitWithOutSpaces.find_first_not_of("0123456789") != std::string::npos) && std::atoi(limitWithOutSpaces.c_str()) > 0);
}

std::string Server::channel_limit(std::vector<std::string> tokens, Channel *channel, size_t &pos, char opera, int fd, std::string chain, std::string &arguments)
{
    std::string limit;
    std::string param;

    param.clear();
    limit.clear();
    if (opera == '+')
    {
        if (tokens.size() > pos)
        {
            limit = tokens[pos++];
            if (!isvalidLimit(limit))
            {
                sendResponse(ERR_INVALIDMODEPARM(channel->GetName(), "(l)"), fd);
            }
            else
            {
                channel->setModeAtindex(4, true);
                channel->SetLimit(std::atoi(limit.c_str()));
                if (!arguments.empty())
                    arguments += " ";
                arguments += limit;
                param = modeToAppend(chain, opera, 'l');
            }
        }
        else
            sendResponse(ERR_NEEDMODEPARM(channel->GetName(), "(l)"), fd);
    }
    else if (opera == '-' && channel->getModeAtindex(4))
    {
        channel->setModeAtindex(4, false);
        channel->SetLimit(0);
        param = modeToAppend(chain, opera, 'l');
    }
    return param;
}