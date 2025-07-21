/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:03:43 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/21 16:04:05 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleKICK(Client &client, std::istringstream &iss)
{
    std::string channelName, targetNick, reason;

    if (!(iss >> channelName >> targetNick))
    {
        client.sendReply("461", "KICK :Not enough parameters");
        return;
    }

    std::getline(iss, reason);
    if (!reason.empty())
    {
        size_t start = reason.find_first_not_of(' ');
        if (start != std::string::npos)
        {
            reason = reason.substr(start);
            if (!reason.empty() && reason[0] == ':')
            {
                reason = reason.substr(1);
            }
        }
        else
        {
            reason.clear();
        }
    }

    std::map<std::string, Channel>::iterator it = _channels.find(channelName);
    if (it == _channels.end())
    {
        client.sendReply("403", channelName + " :No such channel");
        return;
    }
    Channel &channel = it->second;

    if (!channel.isOperator(&client))
    {
        client.sendReply("482", channelName + " :You're not a channel operator");
        return;
    }

    Client *targetClient = NULL;
    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        if (_clients[i].getFd() > 0 && _clients[i].getNickname() == targetNick)
        {
            if (channel.getMembers().count(&_clients[i]))
            {
                targetClient = &_clients[i];
                break;
            }
        }
    }

    if (!targetClient)
    {
        client.sendReply("441", targetNick + " " + channelName + " :They aren't on that channel");
        return;
    }

    std::string kickMsg = ":" + client.getNickname() + " KICK " + channelName + " " + targetNick;
    if (!reason.empty())
    {
        kickMsg += " :" + reason;
    }
    kickMsg += "\r\n";
    channel.broadcast(kickMsg);
    channel.removeMember(targetClient);
    channel.removeOperator(targetClient);

    targetClient->sendMessage(":" + client.getNickname() + " KICK " + channelName + " :" + (reason.empty() ? "Kicked" : reason) + "\r\n");
}