/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 11:27:54 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/09 12:17:55 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleTOPIC(Client &client, std::istringstream &iss)
{
	std::string channelName, topic;
	iss >> channelName >> topic;
	
	if (channelName.empty())
	{
		client.sendReply("461", "TOPIC :Not enough parameters");
		return;
	}
	
	if (_channels.find(channelName) == _channels.end())
	{
		client.sendReply("403", channelName + " :No such channel");
		return;
	}

	if (topic.empty())
	{
		if (_channels[channelName].getName() == "")
			client.sendReply("331", channelName + " :No topic is set");
		else
			client.sendReply("332", channelName + " :" + _channels[channelName].getTopic());
		return;
	}

	topic = topic.substr(1); // Remove leading ':'


	if (!_channels[channelName].isOperator(&client) && _channels[channelName].hasModeTopic())
	{
		client.sendReply("482", channelName + " :You're not channel operator");
		return;
	}

	_channels[channelName].setTopic(topic);
	_channels[channelName].broadcast(":" + client.getNickname() + " TOPIC " + channelName + " :" + topic + "\r\n");
}
