#include "server.hpp"

// bool Server::ServerRunning = true;

Server::Server() {
	Port = 8080;
	ServerSocketFd = -1;
}

void Server::Start() {
	ServerSocket();
	AcceptClient();
}
