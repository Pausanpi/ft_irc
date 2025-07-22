/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickUser.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/09 11:23:58 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleNICK(Client &client, std::istringstream &iss)
{
	std::string nick;
	iss >> nick;

	if (nick.empty())
	{
		client.sendReply("431", "NICK :No nickname given");
		return;
	}

	if (client.getNickname() == nick) {
		return ;
	}

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (_clients[i].getFd() > 0 && _clients[i].getNickname() == nick)
		{
			client.sendReply("433", "NICK :Nickname is already in use");
			return;
		}
	}

	client.setNickname(nick);
	client.setNickOK();
	
	if (client.isRegistered())
	{
		sendWelcomeMessages(client);
	}
}

void CommandHandler::handleUSER(Client &client, std::istringstream &iss)
{
	std::string user, a, b, real;
	iss >> user >> a >> b;
	std::getline(iss, real);

	if (user.empty() || a.empty() || b.empty())
	{
		client.sendReply("461", "USER :Not enough parameters");
		return;
	}

	client.setUsername(user);
	client.setUserOK();

	if (client.isRegistered())
	{
		sendWelcomeMessages(client);
	}
}

void CommandHandler::handleChangeNICK(Client &client, std::istringstream &iss)
{
	std::string nick, lastNick;
	iss >> nick;

	lastNick = client.getNickname();

	if (nick.empty())
	{
		client.sendReply("431", "NICK :No nickname given");
		return;
	}
	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (_clients[i].getFd() > 0 && _clients[i].getNickname() == nick)
		{
			client.sendReply("433", "NICK :Nickname is already in use");
			return;
		}
	}

	client.setNickname(nick);
	client.setNickOK();

	std::ostringstream oss;
	oss << ":" << lastNick << "!" << lastNick << "@localhost" << " NICK :" << client.getNickname() << "\r\n";
	client.sendMessage(oss.str());
}

void CommandHandler::sendWelcomeMessages(Client &client)
{
	std::string message001 = ":Welcome to the Internet Relay Network " 
		+ client.getNickname() 
		+ "!" 
		+ client.getUsername() 
		+ "@localhost";
	client.sendReply("001", message001);

	std::string message002 = ":Your host is ft_irc, running version 1.0";
	client.sendReply("002", message002);

	std::string message003 = ":This server was created today";
	client.sendReply("003", message003);

	std::string message004 = "ft_irc 1.0 o itkol";
	client.sendReply("004", message004);

	std::string message005 = "CHANTYPES=# PREFIX=(o)@ NETWORK=ft_irc NICKLEN=9 CHANNELLEN=50 :are supported by this server";
	client.sendReply("005", message005);
}