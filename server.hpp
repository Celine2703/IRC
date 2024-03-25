#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <vector>

class Server {
public:
	
	Server();

	void Start();
	void ServerSocket();
	void AcceptClient();
	void ReceiveData(int fd);

	static void SignalHandler(int signal);

	void CloseFds();
	void ClearClients(int fd);
private:
	static bool ServerRunning;

	int Port;
	int ServerSocketFd;
	
	std::vector<int> Clients;
	std::vector<struct pollfd> PollFds;
};

#endif