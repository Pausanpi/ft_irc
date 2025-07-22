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
			//revisar lista de operadores de canal y si está vacio darle operador a un random
			if (channel.getMembers().empty()) {
				_channels.erase(it++);
				continue;
			} else if (channel.getOperators().empty()) {
				Client *newOp = *channel.getMembers().begin(); // Asignar el primer miembro como nuevo operador
				channel.addOperator(newOp);
				channel.broadcast(":" + newOp->getNickname() + " MODE " + channel.getName() + " +o " + newOp->getNickname() + "\r\n");
			}
		}
		it++;
	}

	client.sendMessage("ERROR :Closing Link: " + client.getNickname() + " (Quit: " + reason + ")\r\n");
	std::cout << "Client " << client.getNickname() << " has quit: " << reason << std::endl;
	client.setUnRegistered();
}