/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:50 by ranki             #+#    #+#             */
/*   Updated: 2024/04/16 22:27:04 by ranki            ###   ########.fr       */
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
	
	void						parseCommand(std::string cmd, int fd);
	void						PASS(int fd, std::string cmd);
	void						JOIN(std::string cmd, int fd);
	void						PRIVMSG(std::string cmd, int fd);
	void						TOPIC(std::string &cmd, int &fd);
	void						KICK(std::string cmd, int fd);
	void						PART(std::string cmd, int fd);
	void						QUIT(std::string cmd, int fd);
	void						PING(std::string &cmd, int &fd);
	void						INVITE(std::string &cmd, int &fd);
	void						Start(std::string password, int port);
	
	void						ServerSocket();
	void						acceptClient();
	void						receiveData(int fd);
	void						closeFds();
	void						clearClients(int fd);
	void						serverInit();
	void						sendResponse(std::string response, int fd);
	void						setClientNickname(std::string cmd, int fd);
	void						setClientUsername(std::string cmd, int fd);
	void						senderror(int code, std::string Clientname, int fd, std::string msg);
	void 						senderror(int code, std::string Clientname, std::string channelname, int fd, std::string msg);
	void						isRealChannel(std::vector<std::pair<std::string, std::string> > &token, int i, int j, int fd);
	void						isNotRealChannel(std::vector<std::pair<std::string, std::string> > &token, int i, int fd);
	void						checkChannelsForClients(std::vector<std::string> &tmp, int fd);
	void						handleTopicDisplay(Channel* channel, const std::string& channelName, int fd) ;
	void						updateTopic(Channel* channel, const std::string& channelName, const std::string& topic, int fd);
    void						cleanAllChannelNames(std::vector<std::string>& tmp, int fd);
    void						splitChannelNames(const std::string& str, std::vector<std::string>& tmp);
    void						kickOutChannel(const std::string& user, const std::string& reason, std::vector<std::string>& tmp, int fd);
	void						cleanChannels(std::vector<std::string> &channels, int fd);
	void						getGoodReason(std::string &reason);
	void 						extractChannels(std::string &str, std::vector<std::string> &channels);
	void						partOfChannel(std::vector<std::string>& channels, std::string& reason, int fd);
	void						manageChannelInterraction(const std::string& channel, const std::string& reason, int fd, size_t channelIndex);
	void						removeFromChannel(int fd, size_t channelIndex);
	void						removeFd(int fd);
	void						removeClient(int fd);
	void						rmChannels(int fd);
	void						removeClientChannel(int fd, const std::string& reason);
	void						showDisconnectionClient(int fd);
	void						freeClient(int fd);
	void						splitStringByComma(std::string str1, std::vector<std::string>& tmp);
	void						removeEmptyStrings(std::vector<std::string>& tmp);
	void						sendMessageToChannel(const std::string& channel, const std::string& message, int fd);
	void						sendMessageToUser(const std::string& user, const std::string& message, int fd);
	void						distributeMessages(const std::vector<std::string>& recipients, const std::string& message, int fd);
	
	static void					signalHandler(int signal);

	std::string					removeFirstBackLine(std::string);
	std::string					removeAllNewLines(std::string);
	std::string					tTopic();
	std::string					getTopic(std::string &input);
	std::string					tokenizationKickCommand(std::string cmd, std::vector<std::string> &tmp, std::string &user, int fd);
	std::string					extractReason(std::string& reason);
	std::string					extractQuitReason(std::string cmd);
	std::string					initialSplitAndValidate(std::string cmd, std::vector<std::string>& tmp);
	std::string					SplitCmdPrivmsg(std::string cmd, std::vector<std::string> &tmp);
	std::string					adjustOriginalString(std::string str);
	
	bool						isValidNickname(std::string& nickname);
	bool						nicknameAlreadyUseByClient(std::string& nickname);
	bool						hasSufficientParameters(const std::vector<std::string>& scmd, int fd);
	bool						checkParameters(const std::string& user, int fd);
	bool						verifyParameters(std::string cmd, std::vector<std::string>& tmp, std::string& reason, int fd);
	bool						checkChannelMembershipAndRights(std::vector<std::string>& scmd, int& fd);
	bool						checkParameters(std::vector<std::string>& scmd, int& fd);
	bool						handleChannelLimitsAndInvitations(std::vector<std::string>& scmd, int& fd);
	bool						validateMessageAndRecipients(const std::vector<std::string>& tmp, const std::string& message, int fd);
	
	int							tokenizationPartCommand(std::string cmd, std::vector<std::string> &tmp, std::string &reason, int fd);
	int							getPositionColon(std::string &cmd);
	int							findClientByName(std::string nickname);
	int 						tokenizationJoin(std::vector< std::pair <std::string, std::string> > &token, std::string cmd, int fd);

	Channel						*findChannelByName(std::string name);
	Channel						*validateChannel(const std::string& channelName, int fd);
	Client						*findClientByFd(int fd);
	Client						*findClientByNick(std::string nickname);
	std::vector<std::string>	tokenizationCommand(std::string& cmd);
	static void					SetserverRunning(bool value);
	static bool					getServerRunning();

};

#endif