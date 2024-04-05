#include "../server.hpp"
#include <poll.h>
#include <signal.h>
#include "../client.hpp"

std::string removeNewline(std::string str) {
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}

bool Server::is_validNickname(std::string& nickname)
{
	if(!nickname.empty() && (nickname[0] == '&' || nickname[0] == '#' || nickname[0] == ':')) {
		return false;
    }
	for(size_t i = 1; i < nickname.size(); i++)
	{
		if(!std::isalnum(nickname[i]) && nickname[i] != '_' && nickname[i] != '\n') {
			return false;
        }
	}
	return true;
}

bool Server::nicknameAlreadyUseByClient(std::string& nickname)
{
	for (size_t i = 0; i < this->Clients.size(); i++)
	{
		if (this->Clients[i].getNickname() == nickname)
			return true;
	}
	return false;
}


// NICK
void Server::setClientNickname(std::string cmd, int fd)
{
    std::string inuse;
	Client *cli = GetClient(fd);
    cmd = this->removeFirstBackLine(cmd);
	if(cmd.empty()) {
        sendResponse(RED + ERR_NOTENOUGHPARAM(std::string("*")) + WHI, fd);
            return;
    }
	if (nicknameAlreadyUseByClient(cmd) && cli->getNickname() != cmd){
		inuse = "*";
		if(cli->getNickname().empty())
			cli->setNickname(inuse);
	    sendResponse(RED + ERR_NICKINUSE(std::string(cmd)) + WHI, fd); 
		return;
	}
	if(!is_validNickname(cmd)) {
		sendResponse(RED + ERR_ERRONEUSNICK(std::string(cmd)) + WHI, fd);
		return;
	}
	else
	{
		if(cli && cli->isRegistered())
		{
			std::string oldnick = cli->getNickname();
			cli->setNickname(cmd);
			if(!oldnick.empty() && oldnick != cmd)
			{
				if(oldnick == "*" && !cli->getUsername().empty())
				{
					// cli->setLogedin(true);
					sendResponse(GRE + RPL_CONNECTED(cli->getNickname()) + WHI, fd);
					sendResponse(GRE + RPL_NICKCHANGE(cli->getNickname(),cmd) + WHI, fd);
				}
				else
					sendResponse(GRE + RPL_NICKCHANGE(oldnick,cmd) + WHI, fd);
				return;
			}
			
		}
		else if (cli && !cli->isRegistered())
			sendResponse(RED + ERR_NOTREGISTERED(cmd) + WHI, fd);
	}
	// if(cli && cli->isRegistered() && !cli->getNickname().empty() && !cli->getNickname().empty() && cli->getNickname() != "*" && !cli->GetLogedIn())
	if(cli && cli->isRegistered() && !cli->getNickname().empty() && !cli->getNickname().empty() && cli->getNickname() != "*")
	{
		// cli->setLogedin(true);
		sendResponse(GRE + RPL_CONNECTED(cli->getNickname()) + WHI, fd);
	}
}