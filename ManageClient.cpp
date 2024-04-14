#include "../includes/server.hpp"

Client *Server::GetClientNick(std::string nickname)
{
	for (size_t i = 0; i < this->Clients.size(); i++){
		if (this->Clients[i].getNickname() == nickname)
			return &this->Clients[i];
	}
	return NULL;
}