/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 12:26:25 by pausanch         ###   ########.fr       */
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

    // Verificar si el canal existe
    bool isNewChannel = (_channels.find(chanName) == _channels.end());
    
    if (isNewChannel) {
        // Crear canal y hacer al creador operador
        _channels[chanName] = Channel(chanName);
        _channels[chanName].addMember(&client);
        _channels[chanName].addOperator(&client);
    }

    Channel& channel = _channels[chanName];

    // --- Verificaciones para canales existentes ---
    if (!isNewChannel) {
        // 1. ¿El canal es +i y el cliente NO está invitado?
        if (channel.hasMode('i') && !channel.isInvited(&client)) {
            client.sendMessage(":irc 473 " + client.getNickname() + " " + chanName + " :Cannot join channel (+i)\r\n");
            return;
        }

        // 2. ¿El cliente ya está en el canal?
        if (channel.getMembers().count(&client)) {
            client.sendMessage(":irc 443 " + client.getNickname() + " " + chanName + " :is already on channel\r\n");
            return;
        }
    }

    // --- Unir al cliente ---
    channel.addMember(&client);
    
    // Si fue invitado, limpiar la invitación (opcional)
    if (channel.isInvited(&client)) {
        channel.removeInvited(&client);
    }

    // Broadcast del JOIN a todos en el canal
    std::string joinMsg = ":" + client.getNickname() + " JOIN " + chanName + "\r\n";
    channel.broadcast(joinMsg);
}
