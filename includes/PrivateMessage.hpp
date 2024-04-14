/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PrivateMessage.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ranki <ranki@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 17:51:45 by ranki             #+#    #+#             */
/*   Updated: 2024/04/14 17:51:46 by ranki            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PRIVATEMESSAGE_HPP
#define PRIVATEMESSAGE_HPP

#include "../command.hpp"

class PrivateMessage : public Commands {
public:
    PrivateMessage(); // Constructeur par défaut
    ~PrivateMessage(); // Destructeur
    PrivateMessage(const PrivateMessage& other); // Opérateur de recopie
    PrivateMessage& operator=(const PrivateMessage& other); // Constructeur par copie
    void execute() override; // Fonction execute
};

#endif // PRIVATEMESSAGE_HPP