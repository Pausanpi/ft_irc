/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:53:43 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 12:06:18 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleMODE(Client &client, std::istringstream &iss) {
    std::string target, mode, nick;
    iss >> target >> mode >> nick;

    if (target[0] != '#') {
        client.sendMessage(":irc 403 " + target + " :Not a channel\r\n");
        return;
    }

    Channel &channel = _channels[target];
    if (!channel.isOperator(&client)) {
        client.sendMessage(":irc 482 " + target + " :You're not a channel operator\r\n");
        return;
    }

    if (mode == "+o") {
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (_clients[i].getNickname() == nick) {
                channel.addOperator(&_clients[i]);
                std::string msg = ":" + client.getNickname() + " MODE " + target + " +o " + nick + "\r\n";
                channel.broadcast(msg);  // Asegúrate de implementar broadcast en Channel
                return;
            }
        }
        client.sendMessage(":irc 401 " + nick + " :No such nick\r\n");
    }
	else if (mode == "+i"){
		channel.addMode('i');
		channel.broadcast(":" + client.getNickname() + " MODE " + target + " +i\r\n");
	}
}