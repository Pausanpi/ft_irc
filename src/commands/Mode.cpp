/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 16:53:43 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/09 12:16:42 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/CommandHandler.hpp"

void CommandHandler::handleMODE(Client &client, std::istringstream &iss)
{
	std::string target, modestring;
	iss >> target >> modestring;

	if (target.empty()) {
		client.sendReply("461", "MODE :Not enough parameters");
		return;
	}

	if (target[0] != '#')
	{
		client.sendReply("403", target + " :Not a channel");
		return;
	}

	if (_channels.find(target) == _channels.end()) {
		client.sendReply("403", target + " :No such channel");
		return;
	}

	Channel &channel = _channels[target];

	if (modestring.empty()) {
		std::string modes = channel.getModes();
		// Si no hay modos explícitos, mostrar solo el '+'
		if (modes.empty()) {
			client.sendReply("324", target + " +");
		} else {
			client.sendReply("324", target + " +" + modes);
		}
		return;
	}

	if (!channel.isOperator(&client))
	{
		client.sendReply("482", target + " :You're not a channel operator");
		return;
	}

	processModeString(client, channel, target, modestring, iss);
}

void CommandHandler::processModeString(Client &client, Channel &channel, const std::string &target, 
									  const std::string &modestring, std::istringstream &iss) {
	bool adding = true;
	ModeChange result;
	
	for (size_t i = 0; i < modestring.length(); ++i) {
		char c = modestring[i];
		
		if (c == '+') {
			adding = true;
		} else if (c == '-') {
			adding = false;
		} else {
			ModeChange singleChange;
			bool success = false;
			
			if (c == 'o') {
				success = handleOperatorMode(client, channel, c, adding, iss, singleChange);
			} else {
				success = handleChannelMode(client, channel, c, adding, iss, singleChange);
			}
			
			if (success) {
				result.changes += singleChange.changes;
				if (!singleChange.params.empty()) {
					if (!result.params.empty()) result.params += " ";
					result.params += singleChange.params;
				}
			}
		}
	}

	if (!result.changes.empty()) {
		std::string msg = ":" + client.getNickname() + " MODE " + target + " " + result.changes;
		if (!result.params.empty()) {
			msg += " " + result.params;
		}
		msg += "\r\n";
		channel.broadcast(msg);
	}
}

bool CommandHandler::handleChannelMode(Client &client, Channel &channel, char mode, bool adding, 
									   std::istringstream &iss, ModeChange &result) {
	switch (mode) {
		case 'i':
			return handleInviteMode(client, channel, adding, result);
		case 't':
			return handleTopicMode(client, channel, adding, result);
		case 'k':
			return handleKeyMode(client, channel, adding, iss, result);
		case 'l':
			return handleLimitMode(client, channel, adding, iss, result);
		default:
			client.sendReply("472", std::string(1, mode) + " :is unknown mode char to me");
			return false;
	}
}

bool CommandHandler::handleOperatorMode(Client &client, Channel &channel, char mode, bool adding, 
										std::istringstream &iss, ModeChange &result) {
	if (mode == 'o') {
		return handleOperatorPrivilege(client, channel, adding, iss, result);
	}
	return false;
}

bool CommandHandler::handleInviteMode(Client &client, Channel &channel, bool adding, ModeChange &result) {
	(void)client;
	if (adding) {
		channel.addMode('i');
		result.changes += "+i";
	} else {
		channel.removeMode('i');
		result.changes += "-i";
	}
	return true;
}

bool CommandHandler::handleTopicMode(Client &client, Channel &channel, bool adding, ModeChange &result) {
	(void)client;
	if (adding) {
		channel.setModeTopic(true);
		channel.addMode('t');
		result.changes += "+t";
	} else {
		channel.setModeTopic(false);
		channel.removeMode('t');
		result.changes += "-t";
	}
	return true;
}

bool CommandHandler::handleKeyMode(Client &client, Channel &channel, bool adding, std::istringstream &iss, ModeChange &result) {
	if (adding) {
		std::string key;
		iss >> key;
		if (key.empty()) {
			client.sendReply("461", "MODE :Not enough parameters");
			return false;
		}
		channel.setKey(key);
		result.changes += "+k";
		result.params = key;
	} else {
		channel.removeKey();
		result.changes += "-k";
	}
	return true;
}

bool CommandHandler::handleLimitMode(Client &client, Channel &channel, bool adding, std::istringstream &iss, ModeChange &result) {
	if (adding) {
		std::string limitStr;
		iss >> limitStr;
		if (limitStr.empty()) {
			client.sendReply("461", "MODE :Not enough parameters");
			return false;
		}
		int limit = std::atoi(limitStr.c_str());
		if (limit <= 0) {
			client.sendReply("461", "MODE :Invalid limit parameter");
			return false;
		}
		channel.setlimit(limit);
		result.changes += "+l";
		result.params = limitStr;
	} else {
		channel.removelimit();
		result.changes += "-l";
	}
	return true;
}

bool CommandHandler::handleOperatorPrivilege(Client &client, Channel &channel, bool adding, std::istringstream &iss, ModeChange &result) {
	std::string nick;
	iss >> nick;
	if (nick.empty()) {
		client.sendReply("461", "MODE :Not enough parameters");
		return false;
	}
	
	Client* targetClient = findClientByNick(nick);
	if (!targetClient) {
		client.sendReply("401", nick + " :No such nick");
		return false;
	}

	if (adding) {
		channel.addOperator(targetClient);
		result.changes += "+o";
	} else {
		channel.removeOperator(targetClient);
		result.changes += "-o";
	}
	result.params = nick;
	return true;
}