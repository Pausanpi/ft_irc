/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 12:53:59 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/27 13:06:54 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handlePASS(Client &client, std::istringstream &iss, const std::string &password) {
	std::string pass;
	iss >> pass;

	if (pass == password) {
		client.setAutenticated(true);
	}
}