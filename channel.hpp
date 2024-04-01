#ifndef CHANNEL_HPP
# define CHANNEL_HPP
#endif
#include "server.hpp"
#include "client.hpp"

class Channel {
    private : 
        std::string name;
        std::vector<Client> clients;
        std::vector<std::string> messages;
    public :
        Channel(std::string name);
        void addClient(Client client);
        void removeClient(Client client);
        void addMessage(std::string message);
        std::string getName();
        std::vector<Client> getClients();
        std::vector<std::string> getMessages();
};