/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Privmsg.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalado- <jsalado-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:00:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/06/16 13:04:40 by jsalado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handlePRIVMSG(Client &client, std::istringstream &iss)
{
    std::string target;
    iss >> target;

    if (target.empty()) {
        client.sendReply("411", ":No recipient given (PRIVMSG)");
        return;
    }

    std::string msg;
    std::string peek;
    iss >> peek;
    
    if (!peek.empty() && peek[0] == ':') {
        msg = peek.substr(1);
        std::string rest;
        std::getline(iss, rest);
        if (!rest.empty()) {
            msg += rest;
        }
    } else {
        msg = peek;
        std::string rest;
        std::getline(iss, rest);
        if (!rest.empty()) {
            msg += rest;
        }
    }

    if (msg.empty()) {
        client.sendReply("412", ":No text to send");
        return;
    }

    std::ostringstream oss;
    oss << ":" << client.getNickname() << "!" << client.getNickname()
        << "@localhost" << " PRIVMSG " << target << " :" << msg << "\r\n";
    std::string fullMsg = oss.str();

    if (!target.empty() && target[0] == '#')
    {
        std::map<std::string, Channel>::iterator it = _channels.find(target);
        if (it != _channels.end())
        {
            Channel &channel = it->second;
            const std::set<Client *> &members = channel.getMembers();

            if (members.find(&client) == members.end())
            {
                client.sendReply("404", target + " :Cannot send to channel");
                return;
            }

            for (std::set<Client *>::const_iterator iter = members.begin(); iter != members.end(); ++iter)
            {
                Client *member = *iter;
                if (member->getFd() != client.getFd())
                {
                    member->sendMessage(fullMsg);
                }
            }
        }
        else
        {
            client.sendReply("403", target + " :No such channel");
        }
    }
    else
    {
        bool found = false;
        for (int i = 0; i < MAX_CLIENTS; ++i)
        {
            if (_clients[i].getFd() > 0 && _clients[i].getNickname() == target)
            {
                _clients[i].sendMessage(fullMsg);
                found = true;
                break;
            }
        }
        if (!found)
        {
            client.sendReply("401", target + " :No such nick/channel");
        }
    }
}