/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/10 12:01:11 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleQUIT(Client &client, std::istringstream &iss) {
	std::string reason;
	getline(iss, reason);
    
	if (reason.empty())
		reason = "Client Quit";
	else
		reason = reason.substr(reason.find_first_not_of(" :"));

	std::map<std::string, Channel>::iterator it;
	it = _channels.begin();
	while (it != _channels.end()) {
		Channel &channel = it->second;
		if (channel.getMembers().count(&client)) {
			channel.broadcast(":" + client.getNickname() + " QUIT :" + reason + "\r\n");
			channel.removeMember(&client);
		}
		it++;
	}

	std::cout << "Client " << client.getNickname() << " has quit: " << reason << std::endl;
	client.setUnRegistered();
	client.clear();
}