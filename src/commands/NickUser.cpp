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
		std::ostringstream oss;
		oss << ":irc 001 " << client.getNickname()
			<< " :Welcome to ft_irc, " << client.getNickname() << "!\r\n";
		client.sendMessage(oss.str());
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
		std::ostringstream oss;
		oss << ":irc 001 " << client.getNickname()
			<< " :Welcome to ft_irc, " << client.getNickname() << "!\r\n";
		client.sendMessage(oss.str());
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