/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:53:43 by pausanch          #+#    #+#             */
/*   Updated: 2025/06/11 15:25:14 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleMODE(Client &client, std::istringstream &iss) {
    std::string target, mode, extra;
    iss >> target >> mode >> extra;

    if (target[0] != '#') {
        client.sendMessage(":irc 403 " + target + " :Not a channel\r\n");
        return;
    }

    Channel &channel = _channels[target];
    if (!channel.isOperator(&client)) {
        client.sendMessage(":irc 482 " + target + " :You're not a channel operator\r\n");
        return;
    }

    if (mode == "+o"){
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (_clients[i].getNickname() == extra) {
                channel.addOperator(&_clients[i]);
                std::string msg = ":" + client.getNickname() + " MODE " + target + " +o " + extra + "\r\n";
                channel.broadcast(msg);
                return;
            }
        }
        client.sendMessage(":irc 401 " + extra + " :No such extra\r\n");
    }
	else if (mode == "-o"){
		for (int i = 0; i < MAX_CLIENTS; ++i) {
			if (_clients[i].getNickname() == extra && channel.isOperator(&_clients[i])) {
				channel.removeOperator(&_clients[i]);
				std::string msg = ":" + client.getNickname() + " MODE " + target + " -o " + extra + "\r\n";
                channel.broadcast(msg);
                return;
			}
		}
		client.sendMessage(":irc X " + extra + " :Mensajito que no se cual debería poner :)\r\n");
	}
	
	if (mode == "+i"){
		channel.addMode('i');
		channel.broadcast(":" + client.getNickname() + " MODE " + target + " +i\r\n");
	}
	else if (mode == "-i"){
		channel.removeMode('i');
		channel.broadcast(":" + client.getNickname() + " MODE " + target + " -i\r\n");
	}

	if (mode == "+k"){
		channel.setKey(extra);
		channel.broadcast(":" + client.getNickname() + " MODE " + target + " +k " + extra + "\r\n");
	}
	else if (mode == "-k"){
		channel.removeKey();
		channel.broadcast(":" + client.getNickname() + " MODE " + target + " -k\r\n");
	}
}