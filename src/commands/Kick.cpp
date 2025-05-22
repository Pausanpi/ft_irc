/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:03:43 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 16:04:05 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleKICK(Client &client, std::istringstream &iss) {
    std::string channelName, targetNick, reason;
    
    // 1. Leer parámetros obligatorios
    if (!(iss >> channelName >> targetNick)) {
        client.sendMessage(":irc 461 " + client.getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }

    // 2. Leer razón (manejo seguro)
    std::getline(iss, reason);
    if (!reason.empty()) {
        size_t start = reason.find_first_not_of(' ');
        if (start != std::string::npos) {
            reason = reason.substr(start);
            if (!reason.empty() && reason[0] == ':') {
                reason = reason.substr(1);
            }
        } else {
            reason.clear();
        }
    }

    // 3. Validar canal
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        client.sendMessage(":irc 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return;
    }
    Channel &channel = it->second;

    // 4. Validar operador
    if (!channel.isOperator(&client)) {
        client.sendMessage(":irc 482 " + client.getNickname() + " " + channelName + " :You're not a channel operator\r\n");
        return;
    }

    // 5. Buscar target
    Client *targetClient = NULL;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clients[i].getFd() > 0 && _clients[i].getNickname() == targetNick) {
            if (channel.getMembers().count(&_clients[i])) {
                targetClient = &_clients[i];
                break;
            }
        }
    }

    if (!targetClient) {
        client.sendMessage(":irc 441 " + client.getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n");
        return;
    }

    // 6. Ejecutar KICK
    std::string kickMsg = ":" + client.getNickname() + " KICK " + channelName + " " + targetNick;
    if (!reason.empty()) {
        kickMsg += " :" + reason;
    }
    kickMsg += "\r\n";

    channel.broadcast(kickMsg);
    channel.removeMember(targetClient);
    channel.removeOperator(targetClient);

    // Mensaje directo al expulsado
    targetClient->sendMessage(":" + client.getNickname() + " KICK " + channelName + " :" + (reason.empty() ? "Kicked" : reason) + "\r\n");
}