/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Invite.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:20:48 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 12:14:19 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleINVITE(Client &inviter, std::istringstream &iss) {
    std::string targetNick, channelName;
    iss >> targetNick >> channelName;

    Channel &channel = _channels[channelName];
    
    // Verificaciones
    if (!channel.isOperator(&inviter)) {
        inviter.sendMessage(":irc 482 " + channelName + " :You're not a channel operator\r\n");
        return;
    }

    Client* target = findClientByNick(targetNick); // Implementa esta función
    if (!target) {
        inviter.sendMessage(":irc 401 " + targetNick + " :No such nick\r\n");
        return;
    }

    // Envía invitación
    channel.addInvited(target);
    target->sendMessage(":" + inviter.getNickname() + " INVITE " + targetNick + " " + channelName + "\r\n");
    inviter.sendMessage(":irc 341 " + inviter.getNickname() + " " + targetNick + " " + channelName + "\r\n");
}