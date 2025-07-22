/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:35:05 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/15 11:59:15 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Channel.hpp"
#include "../includes/Client.hpp"

Channel::Channel() : _name("default"), _inviteOnly(false), _limitmember(0), _modeTopic(false) {}

Channel::Channel(const std::string& name) : _name(name), _inviteOnly(false), _limitmember(0), _modeTopic(false) {}

const std::string& Channel::getName() const {
    return _name;
}

void Channel::addMember(Client* client) {
    _members.insert(client);
}

void Channel::removeMember(Client* client) {
    _members.erase(client);
	_operators.erase(client);
	_invited.erase(client);
}

const std::set<Client*>& Channel::getMembers() const {
    return _members;
}

void Channel::addOperator(Client* client) {
	_operators.insert(client);
}

void Channel::removeOperator(Client* client) {
	_operators.erase(client);
}

const std::set<Client*>& Channel::getOperators() const {
    return _operators;
}

bool Channel::isOperator(Client* client) const {
	return _operators.find(client) != _operators.end();
}

void Channel::broadcast(const std::string &msg) {
	for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if (*it != NULL && (*it)->getFd() > 0) {
			(*it)->sendMessage(msg);
		}
	}
}

void Channel::broadcastToOthers(const std::string &msg, Client* excludeClient) {
	for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if (*it != NULL && (*it)->getFd() > 0 && *it != excludeClient) {
			(*it)->sendMessage(msg);
		}
	}
}

void Channel::addInvited(Client* client) {
	_invited.insert(client);
}

bool Channel::isInvited(Client* client) const {
	return _invited.find(client) != _invited.end();
}

bool Channel::isInviteOnly() const {
	return _inviteOnly;
}

void Channel::removeInvited(Client* client) {
	_invited.erase(client);
}

void Channel::addMode(char mode) {
	if (_modes.find(mode) == std::string::npos) {
		_modes += mode;
	}
}

void Channel::removeMode(char mode) {
	size_t pos = _modes.find(mode);
	if (pos != std::string::npos) {
		_modes.erase(pos, 1);
	}
}

bool Channel::hasMode(char mode) const {
	return _modes.find(mode) != std::string::npos;
}

const std::string& Channel::getModes() const {
	return _modes;
}

void Channel::setKey(const std::string& key) {
	_key = key;
}

void Channel::removeKey() {
	_key.clear();
}

const std::string& Channel::getKey() const {
	return _key;
}

bool Channel::hasKey() const {
	return !_key.empty();
}

void Channel::setlimit(int limit) {
	_limitmember = limit;
}

int Channel::getlimit() {
	return(_limitmember);
}

void Channel::removelimit() {
	_limitmember = 0;
}

int Channel::getnumberofmembers() {
	return(_members.size());
}

std::string Channel::getUserList() const {
	std::string userList = "";
	for (std::set<Client*>::const_iterator it = _members.begin(); it != _members.end(); ++it) {
		if (isOperator(*it)) {
			userList += "@" + (*it)->getNickname() + " ";
		} else {
			userList += (*it)->getNickname() + " ";
		}
	}
	return userList;
}


void Channel::setTopic(const std::string& topic) {
	_topic = topic;	
}

const std::string& Channel::getTopic() const {
	return _topic;
}

bool Channel::hasModeTopic() const {
	return _modeTopic;
}

void Channel::setModeTopic(bool mode) {
	_modeTopic = mode;
}