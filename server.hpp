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

class Server {
public:
	
	Server();

	void Start(int port);
	void ServerSocket();
	void AcceptClient();
	void ReceiveData(int fd);

	static void SignalHandler(int signal);

	void CloseFds();
	void ClearClients(int fd);
	void ServerInit();
private:
	static bool ServerRunning;

	int Port;
	int ServerSocketFd;
	
	std::vector<Client> Clients;
	std::vector<struct pollfd> PollFds;

};

#endif