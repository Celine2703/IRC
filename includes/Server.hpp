/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:50 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 22:48:28 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "Client.hpp"
#include "Channel.hpp"
#include <cstdlib>
#include <sstream>
#include "Replies.hpp"
#include <algorithm>
#include <ctime>
#include <sstream>
#include <cerrno>
#include <cstdio>



#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"


class Server {

private:
	static bool					ServerRunning;

	int							Port;
	int							ServerSocketFd;

	std::string					password;
	std::vector<Client>			Clients;
	std::vector<Channel>		channels;
	std::vector<struct pollfd>	PollFds;
	

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
	void						ParseCommand(std::string cmd, int fd);
	Client						*GetClient(int fd);
	std::string					removeFirstBackLine(std::string);
	std::vector<std::string>	tokenizationCommand(std::string& cmd);
	void						PASS_Client(int fd, std::string cmd);
	void						JOIN_Client(std::string cmd, int fd);
	void						sendResponse(std::string response, int fd);
	void						setClientNickname(std::string cmd, int fd);
	bool						is_validNickname(std::string& nickname);
	bool						nicknameAlreadyUseByClient(std::string& nickname);
	void						setClientUsername(std::string cmd, int fd);
	int 						tokenizationJoin(std::vector< std::pair <std::string, std::string> > &token, std::string cmd, int fd);
	void						senderror(int code, std::string Clientname, int fd, std::string msg);
	void 						senderror(int code, std::string Clientname, std::string channelname, int fd, std::string msg);
	void						ExistCh(std::vector<std::pair<std::string, std::string> > &token, int i, int j, int fd);
	int							SearchForClients(std::string nickname);
	void						NotExistCh(std::vector<std::pair<std::string, std::string> > &token, int i, int fd);
	void						CheckForChannels_Clients(std::vector<std::string> &tmp, int fd);
	void						PRIVMSG(std::string cmd, int fd);
	Channel						*GetChannel(std::string name);
	Client						*GetClientNick(std::string nickname);
	std::string					tTopic();
	std::string					gettopic(std::string &input);
	int							getpos(std::string &cmd);
	void						TOPIC(std::string &cmd, int &fd);
	bool						hasSufficientParameters(const std::vector<std::string>& scmd, int fd);
	Channel						*validateChannel(const std::string& channelName, int fd);
	void						handleTopicDisplay(Channel* channel, const std::string& channelName, int fd) ;
	void						updateTopic(Channel* channel, const std::string& channelName, const std::string& topic, int fd);
	void						KICK(std::string cmd, int fd);
	std::string					SplitCmdKick(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
	std::string					ExtractReason(std::string& reason);
    void						CleanChannelNames(std::vector<std::string>& tmp, int fd);
    void						SplitChannelNames(const std::string& str, std::vector<std::string>& tmp);
	bool						CheckParameters(const std::string& user, int fd);
    void						ProcessKickForChannel(const std::string& user, const std::string& reason, std::vector<std::string>& tmp, int fd);
	void						PART(std::string cmd, int fd);
	void						CleanChannels(std::vector<std::string> &channels, int fd);
	void						NormalizeReason(std::string &reason);
	int							SplitCmdPart(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
	void 						ExtractChannels(std::string &str, std::vector<std::string> &channels);
	bool						VerifyParameters(std::string cmd, std::vector<std::string>& tmp, std::string& reason, int fd);
	void						ProcessChannelPart(std::vector<std::string>& channels, std::string& reason, int fd);
	void						HandleChannelParticipation(const std::string& channel, const std::string& reason, int fd, size_t channelIndex);
	void						RemoveFromChannel(int fd, size_t channelIndex);
	void						QUIT(std::string cmd, int fd);
	void						RemoveFds(int fd);
	void						RemoveClient(int fd);
	void						RmChannels(int fd);
	std::string					ExtractQuitReason(std::string cmd);
	void						ProcessChannelParticipation(int fd, const std::string& reason);
	void						NotifyDisconnection(int fd);
	void						CleanupResources(int fd);

};

#endif