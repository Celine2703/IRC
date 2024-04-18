/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ManageChannel.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:22 by ranki             #+#    #+#             */
/*   Updated: 2024/04/18 10:22:22 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Channel *Server::findChannelByName(std::string name)
{
	for (size_t i = 0; i < this->channels.size(); i++)
	{
		if (removeAllNewLines(this->channels[i].GetName()) == removeAllNewLines(name))
		{
			return &channels[i];
		}
	}
	return NULL;
}