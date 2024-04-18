/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:34 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 11:50:28 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CHANNEL_HPP
# define CHANNEL_HPP

#include "Server.hpp"
#include "Client.hpp"

class Channel
{
	private:

		std::string 						topic_name;
		std::vector<Client>					Clients;
		std::vector<Client>					admins;
		std::vector<std::pair<char, bool> >	modes;
		int 								limit;
		int 								topic;
		std::string 						created_at;
		int 								key;
		bool								isTopicRestriction;
		std::string 						name;
		std::string 						time_creation;
		std::string 						password;
		int 								isOnlyInvite;
		
	public:
		Channel();
		~Channel();
		Channel(Channel const &src);
		
		Channel						&operator=(Channel const &src);
		void 						setInvitOnly(int isOnlyInvite);
		void 						SetTopic(int topic);
		void 						SetKey(int key);
		void 						SetLimit(int limit);
		void 						SetTopicName(std::string topic_name);
		void 						SetPassword(std::string password);
		void 						SetName(std::string name);
		void 						SetTime(std::string time);
		void 						setTopicRestriction(bool value);
		void 						setModeAtindex(size_t index, bool mode);
		void 						setCreateAt();
		int 						GetInvitOnly();
		int 						GetTopic();
		int 						GetKey();
		int 						GetLimit();
		int 						findClientByFdsNumber();
		bool 						getTopicRestriction() const;
		bool 						getModeAtindex(size_t index);
		bool 						ClientInChannel(std::string &nick);
		std::string					GetTopicName();
		std::string					GetPassword();
		std::string					GetName();
		std::string					GetTime();
		std::string					get_creationtime();
		std::string					getModes();
		std::string					clientChannelList();
		Client 						*getClientInChannelByFd(int fd);
		Client 						*getAdmin(int fd);
		Client 						*findClientByFdInChannel(std::string name);
		void 						addClient(Client newClient);
		void 						addAdmin(Client newClient);
		void 						removeClient(int fd);
		void 						removeAdmin(int fd);
		bool 						changeClientToAdmin(std::string& nick);
		bool 						changeAdminToClient(std::string& nick);
		void 						sendToAll(std::string rpl1);
		void 						sendToAll(std::string rpl1, int fd);
		std::vector<Client>&		getClients();
		std::vector<Client>&			getAdmins();

	
		
};

#endif