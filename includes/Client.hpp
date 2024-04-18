/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:37 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 10:05:37 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

class Client {

private:
	int							Fd;
	std::string					IPAddress;
	std::string					nickname;
	std::string					username;
	bool						registered;
	bool						login;
	std::vector<std::string>	channelsInvite;
	std::string					buffer;
	bool						firstMessage;
	int							firstMode;
	
public:
	Client();

// Methods
	void	clearBuffer();

// Getters and setters 
	int				getFd();
	void			setFd(int fd);
	void			setIp(std::string IP_address);
    const			std::string& getNickname() const;
    void			setNickname(const std::string& nickname);
    const			std::string& getUsername() const;
    void			setUsername(const std::string& username);
    bool			isRegistered() const;
    void			setRegistered(bool registered);
    const			std::vector<std::string>& getChannelsInvite() const;
    bool			getChannelInvite(std::string name);
    void			setChannelsInvite(const std::vector<std::string>& channelsInvite);
	void			rmChannelInvite(std::string chname);
	void			addChannelInvite(std::string chname);
    const			std::string& getBuffer() const;
    void			setBuffer(const std::string& buffer);
	bool			isFirstMessage();
	void			setFirstMessage(bool value);
	int				getFirstMode();
	void			setFirstMode(int value);
	bool			isLogin();
	void			setLogin(bool value);
	std::string		getHostname();
	std::string		getIPAddress();
	
};



#endif