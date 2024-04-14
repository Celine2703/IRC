#include "../server.hpp"
#include <poll.h>
#include <signal.h>
#include "../client.hpp"

std::string removeNewline2(std::string str)
{
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	str.erase(std::remove(str.begin(), str.end(), '\r'), str.end());
	return str;
}

// PASS
void Server::PASS_client(int fd, std::string cmd)
{
	Client *cli = GetClient(fd);

	if (cmd.empty())
		sendResponse(RED + ERR_NOTENOUGHPARAM(std::string("*")) + WHI, fd);
	else if (!cli->isRegistered())
	{
		std::string pass = cmd;
		if (removeNewline2(pass) == password)
		{
			cli->setRegistered(true);
			std::string response = GRE + std::string("YOU ARE NOW REGISTERED") + WHI + std::string(CRLF);
			sendResponse(response, fd);
		}
		else
			sendResponse(RED + ERR_INCORPASS(std::string("*")) + WHI, fd);
	}
	else
		sendResponse(RED + ERR_ALREADYREGISTERED(GetClient(fd)->getNickname()) + WHI, fd);
}