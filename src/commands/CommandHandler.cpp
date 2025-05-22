/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/22 11:45:16 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

CommandHandler::CommandHandler(Client* clients, std::map<std::string, Channel>& channels)
    : _clients(clients), _channels(channels){
}

CommandHandler::~CommandHandler() {
    // No es necesario liberar nada aquí ya que no gestionamos memoria
}

Client* CommandHandler::findClientByNick(const std::string& nick) {
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clients[i].getFd() > 0 && _clients[i].getNickname() == nick) {
            return &_clients[i];
        }
    }
    return NULL;
}