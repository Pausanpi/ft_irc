/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandHandler.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/09 11:27:33 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HANDLER_HPP
#define COMMAND_HANDLER_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include "Client.hpp"
#include "Channel.hpp"
#include "Server.hpp"

class CommandHandler {
private:
    Client* _clients;
    std::map<std::string, Channel>& _channels;

public:
    CommandHandler(Client* clients, std::map<std::string, Channel>& channels);
    ~CommandHandler();

    // Métodos para manejar los diferentes comandos
	void handlePASS(Client &client, std::istringstream &iss, const std::string &password);
    void handleNICK(Client &client, std::istringstream &iss);
    void handleUSER(Client &client, std::istringstream &iss);
	void handleChangeNICK(Client &client, std::istringstream &iss);
    void handlePRIVMSG(Client &client, std::istringstream &iss);
    void handleJOIN(Client &client, std::istringstream &iss);
    void handleQUIT(Client &client, std::istringstream &iss);
	void handleMODE(Client &client, std::istringstream &iss);
	void handleKICK(Client &client, std::istringstream &iss);
	void handleINVITE(Client &inviter, std::istringstream &iss);
	void handleTOPIC(Client &client, std::istringstream &iss);

	// Helper methods
	void sendWelcomeMessages(Client &client);


	Client* findClientByNick(const std::string &nick);
};

#endif // COMMAND_HANDLER_HPP