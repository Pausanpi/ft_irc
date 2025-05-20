/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/20 16:12:53 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleNICK(Client &client, std::istringstream &iss) {
    std::string nick;
    iss >> nick;
    client.setNickname(nick);
}

void CommandHandler::handleUSER(Client &client, std::istringstream &iss) {
    std::string user, a, b, real;
    iss >> user >> a >> b;
    std::getline(iss, real);
    client.setUsername(user);
    client.registerUser();

    std::ostringstream oss;
    oss << ":irc 001 " << client.getNickname()
        << " :Welcome to ft_irc, " << client.getNickname() << "!\r\n";
    client.sendMessage(oss.str());
}