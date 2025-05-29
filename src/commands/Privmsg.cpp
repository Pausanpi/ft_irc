/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/29 11:43:21 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handlePRIVMSG(Client &client, std::istringstream &iss) {
    std::string target;
    iss >> target;

    std::string msg;
    std::getline(iss, msg);

    // Remove leading spaces
    size_t pos = msg.find_first_not_of(' ');
    if (pos != std::string::npos) {
        msg = msg.substr(pos);
    } else {
        msg.clear(); // no message content
    }

    // Remove leading ':' if present (common in IRC protocol)
    if (!msg.empty() && msg[0] == ':') {
        msg = msg.substr(1);
    }

    std::ostringstream oss;
    oss << ":" << client.getNickname()
        << " PRIVMSG " << target << " :" << msg << "\r\n";
    std::string fullMsg = oss.str();

    if (!target.empty() && target[0] == '#') {
        // Message to channel
        std::map<std::string, Channel>::iterator it = _channels.find(target);
        if (it != _channels.end()) {
            Channel &channel = it->second;
            const std::set<Client *> &members = channel.getMembers();

            // Check if sender is in the channel members
            if (members.find(&client) == members.end()) {
                // Client is NOT in the channel, reject sending message
                // Optionally send an error message back to client:
                client.sendMessage(":irc 404 " + client.getNickname() + " " + target + " :Cannot send to channel (not a member)\r\n");
                return;
            }

            // Client is in the channel, send the message to all other members
            for (std::set<Client *>::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
                Client *member = *iter;
                if (member->getFd() != client.getFd()) {
                    member->sendMessage(fullMsg);
                }
            }
        } else {
            // Channel not found: optionally handle error
            client.sendMessage(":irc 403 " + client.getNickname() + " " + target + " :No such channel\r\n");
        }
    } else {
        // Message to user
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (_clients[i].getFd() > 0 && _clients[i].getNickname() == target) {
                _clients[i].sendMessage(fullMsg);
                break;
            }
        }
    }
}