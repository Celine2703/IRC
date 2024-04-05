#include "../server.hpp"
#include <poll.h>
#include <signal.h>
#include "../client.hpp"

void Server::setClientUsername(std::string cmd, int fd)
{

	Client *cli = GetClient(fd);
	// if ((cli && cmd.size() < 5))
	// {
	// 	sendResponse(ERR_NOTENOUGHPARAM(cli->getNickname()), fd);
	// 	return;
	// }
	if (!cli || !cli->isRegistered())
		sendResponse(RED + ERR_NOTREGISTERED(std::string("*")) + WHI, fd);
	else if (cli && !cli->getUsername().empty())
	{
		sendResponse(RED + ERR_ALREADYREGISTERED(cli->getNickname()) + WHI, fd);
		return;
	}
	else
	{
		cli->setUsername(cmd);
		sendResponse(GRE + std::string("YOU'RE USERNAME IS SET.\n") + WHI, fd);
	}
	if (cli && cli->isRegistered() && !cli->getUsername().empty() && !cli->getNickname().empty() && cli->getNickname() != "*") // && !cli->GetLogedIn()
	{
		// cli->setLogedin(true);
		sendResponse(GRE + RPL_CONNECTED(cli->getNickname()) + WHI, fd);
	}
}