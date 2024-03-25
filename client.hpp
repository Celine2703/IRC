#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>

class Client {
public:
	Client();

	int getFD();
	void setFD(int fd);
	void setIP(std::string IP_address);

private:
	int Fd;
	std::string IPAddress;
};

#endif