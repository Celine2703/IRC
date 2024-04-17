/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 19:04:14 by ranki             #+#    #+#             */
/*   Updated: 2024/04/17 22:39:49 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

void Server::PING(std::string &cmd, int &fd)
{
    (void)cmd;
    
    sendResponse(": PONG :"+ findClientByFd(fd)->getNickname() + "\n", fd);
}