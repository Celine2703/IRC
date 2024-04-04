#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

class Client {

private:
	int Fd;
	std::string IPAddress;
	std::string nickname;
	std::string username;
	bool registered;
	bool login;
	std::vector<std::string> channelsInvite;
	std::string buffer;

public:
	Client();

// Methods
	void	clearBuffer();

// Getters and setters 
	int		getFD();
	void	setFD(int fd);
	void	setIP(std::string IP_address);
    const	std::string& getNickname() const;
    void	setNickname(const std::string& nickname);
    const	std::string& getUsername() const;
    void	setUsername(const std::string& username);
    bool	isRegistered() const;
    void	setRegistered(bool registered);
    const	std::vector<std::string>& getChannelsInvite() const;
    void	setChannelsInvite(const std::vector<std::string>& channelsInvite);
    const	std::string& getBuffer() const;
    void	setBuffer(const std::string& buffer);
	
};



#endif