/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/15 12:10:53 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleQUIT(Client &client, std::istringstream &iss) {
	std::string reason;
	getline(iss, reason);
    
	if (reason.empty()) {
		reason = "Client Quit";
	} else {
		size_t start = reason.find_first_not_of(" :");
		if (start != std::string::npos) {
			reason = reason.substr(start);
		} else {
			reason = "Client Quit";
		}
	}

	if (client.isRegistered()) {
		std::string quitMessage = ":" + client.getNickname() + "!" + client.getUsername() + "@localhost QUIT :" + reason + "\r\n";
		std::map<std::string, Channel>::iterator it = _channels.begin();
		while (it != _channels.end()) {
			Channel &channel = it->second;
			if (channel.getMembers().count(&client)) {
				channel.broadcastToOthers(quitMessage, &client);
				channel.removeMember(&client);
				if (channel.getMembers().empty()) {
					std::cout << "Server: Channel " << channel.getName() << " is now empty, removing" << std::endl;
					_channels.erase(it++);
					continue;
				} 
				else if (channel.getOperators().empty() && !channel.getMembers().empty()) {
					Client *newOp = *channel.getMembers().begin();
					channel.addOperator(newOp);
					std::string modeMessage = ":irc MODE " + channel.getName() + " +o " + newOp->getNickname() + "\r\n";
					channel.broadcast(modeMessage);
					std::cout << "Server: Auto-promoted " << newOp->getNickname() << " to operator in " << channel.getName() << std::endl;
				}
			}
			++it;
		}
		
		std::cout << "Server: " << client.getNickname() << "!" << client.getUsername() << "@localhost has quit (" << reason << ")" << std::endl;
	} else {
		if (!client.getNickname().empty()) {	
			std::cout << "Server: " << client.getNickname() << " has quit during registration (" << reason << ")" << std::endl;
		} else {
			std::cout << "Server: Unregistered client has quit (" << reason << ")" << std::endl;
		}
	}
	client.setUnRegistered();
	client.clear();
}