/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/14 13:12:00 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

extern std::map<std::string, Channel> _channels;

void CommandHandler::handleJOIN(Client &client, std::istringstream &iss)
{
    std::string chanName;
    iss >> chanName;

    if (chanName.empty())
    {
        client.sendReply("461", "JOIN :Not enough parameters");
        return;
    }
	if (chanName[0] != '#')
	{
		client.sendReply("476", client.getNickname() + " " + chanName + " :Invalid channel name");
		return;
	}
	if (chanName.length() == 1)
	{
		client.sendReply("461", "JOIN :Not enough parameters");
		return;
	}

    bool isNewChannel = (_channels.find(chanName) == _channels.end());

    if (isNewChannel)
    {
        _channels[chanName] = Channel(chanName);
        _channels[chanName].addMember(&client);
        _channels[chanName].addOperator(&client);
		_channels[chanName].setModeTopic(true);
    }

    Channel &channel = _channels[chanName];

    if (!isNewChannel)
    {
        if (channel.hasMode('i') && !channel.isInvited(&client))
        {
            client.sendReply("473", client.getNickname() + " " + chanName + " :Cannot join channel (+i)");
            return;
        }

        if (channel.getMembers().count(&client))
        {
            client.sendReply("443", client.getNickname() + " " + chanName + " :is already on channel");
            return;
        }

        if (channel.hasKey())
        {
            std::string key;
            iss >> key;
            if (key.empty() || key != channel.getKey())
            {
                client.sendReply("475", client.getNickname() + " " + chanName + " :Cannot join channel (+k)");
                return;
            }
        }

        if (channel.getlimit() != 0 && channel.getlimit() <= channel.getnumberofmembers())
        {
            client.sendReply("471", client.getNickname() + " " + chanName + " :Cannot join channel (+l)");
            return;
        }
    }

    channel.addMember(&client);
    if (channel.isInvited(&client))
    {
        channel.removeInvited(&client);
    }
    std::string joinMsg = ":" + client.getNickname() + " JOIN " + chanName + "\r\n";
    channel.broadcastToOthers(joinMsg, &client);
    client.sendMessage(":" + client.getNickname() + " JOIN " + chanName + "\r\n");
	std::string userList = channel.getUserList();
	if (!userList.empty())
		client.sendReply("353", "= " + chanName + " :" + userList);
	client.sendReply("366", chanName + " :End of /NAMES list");

    if (_channels[chanName].getTopic().empty())
		client.sendReply("331", chanName + " :No topic is set");
	else
		client.sendReply("332", chanName + " :" + _channels[chanName].getTopic());
}
