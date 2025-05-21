/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:03:43 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 11:12:27 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleKICK(Client &client, std::istringstream &iss) {
    std::string channelName, targetNick, reason;
    iss >> channelName >> targetNick;
    std::getline(iss, reason); // Opcional: razón del kick

    // Limpia espacios y ':' inicial en la razón (si existe)
    reason = reason.substr(reason.find_first_not_of(' '));
    if (!reason.empty() && reason[0] == ':')
        reason = reason.substr(1);

    // --- Validaciones ---
    // 1. ¿El canal existe?
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        client.sendMessage(":irc 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
        return;
    }
    Channel &channel = it->second;

    // 2. ¿El cliente es operador del canal?
    if (!channel.isOperator(&client)) {
        client.sendMessage(":irc 482 " + client.getNickname() + " " + channelName + " :You're not a channel operator\r\n");
        return;
    }

    // 3. ¿El objetivo (targetNick) está en el canal?
    Client *targetClient = NULL;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clients[i].getNickname() == targetNick && channel.getMembers().count(&_clients[i])) {
            targetClient = &_clients[i];
            break;
        }
    }
    if (!targetClient) {
        client.sendMessage(":irc 441 " + client.getNickname() + " " + targetNick + " " + channelName + " :They aren't on that channel\r\n");
        return;
    }

    // --- Ejecutar el KICK ---
    // Mensaje de kick a todos en el canal (incluyendo al expulsado)
    std::string kickMsg = ":" + client.getNickname() + " KICK " + channelName + " " + targetNick + " :" + (reason.empty() ? "No reason given" : reason) + "\r\n";
    channel.broadcast(kickMsg);

    // Eliminar al cliente del canal
    channel.removeMember(targetClient);
    channel.removeOperator(targetClient); // Si era operador, lo removemos

    // Mensaje opcional al cliente expulsado
    targetClient->sendMessage(targetNick + " KICK " + channelName + " :You've been kicked by " + client.getNickname() + "\r\n");
}