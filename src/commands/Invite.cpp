/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:20:48 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 11:20:55 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleINVITE(Client &inviter, std::istringstream &iss) {
    std::string targetNick, channelName;
    iss >> targetNick >> channelName;

    // --- Validaciones ---
    // 1. ¿El canal existe?
    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end()) {
        inviter.sendMessage(":irc 403 " + inviter.getNickname() + " " + channelName + " :No such channel\r\n");
        return;
    }
    Channel &channel = it->second;

    // 2. ¿El inviter es miembro (y operador) del canal?
    if (!channel.getMembers().count(&inviter)) {
        inviter.sendMessage(":irc 442 " + inviter.getNickname() + " " + channelName + " :You're not on that channel\r\n");
        return;
    }

    // 3. ¿El inviter es operador o el canal no es +i? (opcional, según RFC)
    if (channel.isInviteOnly() && !channel.isOperator(&inviter)) {
        inviter.sendMessage(":irc 482 " + channelName + " :You're not a channel operator\r\n");
        return;
    }

    // 4. ¿El target existe y no está ya en el canal?
    Client *targetClient = NULL;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clients[i].getNickname() == targetNick) {
            targetClient = &_clients[i];
            break;
        }
    }
    if (!targetClient) {
        inviter.sendMessage(":irc 401 " + inviter.getNickname() + " " + targetNick + " :No such nick\r\n");
        return;
    }
    if (channel.getMembers().count(targetClient)) {
        inviter.sendMessage(":irc 443 " + inviter.getNickname() + " " + targetNick + " " + channelName + " :is already on channel\r\n");
        return;
    }

    // --- Ejecutar INVITE ---
    // Mensaje al invitado
    std::string inviteMsg = ":" + inviter.getNickname() + " INVITE " + targetNick + " " + channelName + "\r\n";
    targetClient->sendMessage(inviteMsg);

    // Mensaje de confirmación al inviter
    inviter.sendMessage(":irc 341 " + inviter.getNickname() + " " + targetNick + " " + channelName + "\r\n");

    // Añadir a la lista de invitados (si el canal es +i)
    channel.addInvited(targetClient);
}