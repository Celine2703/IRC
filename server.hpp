#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
#include <netinet/in.h>
#include "client.hpp"
#include <cstdlib>
#include <sstream>
#include "replies.hpp"
#include <algorithm>


#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"


class Server {
public:
	
	Server();

	void						Start(std::string password, int port);
	void						ServerSocket();
	void						AcceptClient();
	void						ReceiveData(int fd);
	static void					SignalHandler(int signal);
	void						CloseFds();
	void						ClearClients(int fd);
	void						ServerInit();
	void						ParseCommmand(std::string cmd, int fd);
	Client						*GetClient(int fd);
	std::string					removeFirstBackLine(std::string);
	std::vector<std::string>	tokenizationCommand(std::string& cmd);
	void						PASS_client(int fd, std::string cmd);
	void						sendResponse(std::string response, int fd);
	void						setClientNickname(std::string cmd, int fd);
	bool						is_validNickname(std::string& nickname);
	bool						nicknameAlreadyUseByClient(std::string& nickname);
	void						setClientUsername(std::string cmd, int fd);

private:
	static bool					ServerRunning;

	int							Port;
	int							ServerSocketFd;

	std::string					password;
	std::vector<Client>			Clients;
	std::vector<struct pollfd>	PollFds;
	

};

#endif