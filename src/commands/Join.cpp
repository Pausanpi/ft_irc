/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/20 17:10:07 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

extern std::map<std::string, Channel> _channels;

void CommandHandler::handleJOIN(Client &client, std::istringstream &iss) {
    std::string chanName;
    iss >> chanName;

    if (chanName.empty()) {
        client.sendMessage(":irc 461 JOIN :Not enough parameters\r\n");
        return;
    }

    // Create channel if it doesn't exist
    if (_channels.find(chanName) == _channels.end()) {
        _channels.insert(std::make_pair(chanName, Channel(chanName)));
		_channels[chanName].addMember(&client);
		_channels[chanName].addOperator(&client);
    }

    // Add client to the channel
    Channel& channel = _channels[chanName];
    channel.addMember(&client);

    std::ostringstream joinMsg;
    joinMsg << ":" << client.getNickname() << " JOIN " << chanName << "\r\n";
    client.sendMessage(joinMsg.str());
}
