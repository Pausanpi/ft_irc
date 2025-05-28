/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:53:59 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/28 14:55:42 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handlePASS(Client &client, std::istringstream &iss, const std::string &password) {
	std::string pass;
	iss >> pass;

	if (pass.empty()) {
		client.sendMessage("461 PASS :Not enough parameters\n");
		return;
	}

	if (pass != password) {
		client.sendMessage("464: Password incorrect\n");
		return;
	}
	
	client.setAutenticated(true);
	client.sendMessage("001: Welcome to the server!\n");
}