/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:22 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 09:32:36 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Channel *Server::findChannelByName(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		std::cout << "on cherche " << name << " alors que c'est " << this->channels[i].GetName() << std::endl;
		if (removeAllNewLines(this->channels[i].GetName()) == removeAllNewLines(name))
		{
			std::cout << "is found " << name << std::endl;
			return &channels[i];
		}
	}
	return NULL;
}