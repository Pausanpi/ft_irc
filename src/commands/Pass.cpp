/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:53:59 by pausanch          #+#    #+#             */
/*   Updated: 2025/06/10 12:37:56 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handlePASS(Client &client, std::istringstream &iss, const std::string &password)
{
	std::string pass;
	iss >> pass;

	if (pass.empty())
	{
		client.sendReply("461", "PASS :Not enough parameters");
		return;
	}

	if (client.getAuthenticated())
	{
		client.sendReply("462", "PASS :Unauthorized command (already registered)");
		return;
	}

	if (pass != password)
	{
		client.sendReply("464", "PASS :Password incorrect");
		return;
	}

	client.setAutenticated(true);
	client.sendMessage("NOTICE AUTH :*** Password accepted. Please provide NICK and USER commands.\r\n");
}