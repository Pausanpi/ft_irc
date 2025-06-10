/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/29 11:42:28 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleNICK(Client &client, std::istringstream &iss) {
    std::string nick;
    iss >> nick;

	if (nick.empty()) {
		client.sendMessage(":irc 431 NICK :No nickname given\r\n");
		return;
	}
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (_clients[i].getFd() > 0 && _clients[i].getNickname() == nick) {
			client.sendMessage(":irc 433 NICK :Nickname is already in use\r\n");
			return; //tendr'ia que ser un return?
		}
	}
	
    client.setNickname(nick);
}

void CommandHandler::handleUSER(Client &client, std::istringstream &iss) {
    std::string user, a, b, real;
    iss >> user >> a >> b;
    std::getline(iss, real);

	if (user.empty() || a.empty() || b.empty()) {
		client.sendMessage(":irc 461 USER :Not enough parameters\r\n");
		return;
	}

    client.setUsername(user);
    client.registerUser();

    std::ostringstream oss;
    oss << ":irc 001 " << client.getNickname()
        << " :Welcome to ft_irc, " << client.getNickname() << "!\r\n";
    client.sendMessage(oss.str());
}