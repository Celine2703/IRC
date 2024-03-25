#include "client.hpp"

Client::Client() {
	Fd = -1;
	IPAddress = "";
}

int Client::getFD() {
	return Fd;
}

void Client::setFD(int fd) {
	this->Fd = fd;
}

void Client::setIP(std::string IP_address) {
	this->IPAddress = IP_address;
}