/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:40:17 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/10 12:06:06 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"
#include <iostream>

Client::Client() : _fd(-1), _registered(false), _authenticated(false), _nickOK(false), _userOK(false) {}

Client::~Client() {}

void Client::setFd(int fd) {
    _fd = fd;
}

int Client::getFd() const {
    return _fd;
}

void Client::setNickname(const std::string& nick) {
    _nickname = nick;
}

const std::string& Client::getNickname() const {
    return _nickname;
}

void Client::setUsername(const std::string& user) {
    _username = user;
}

const std::string& Client::getUsername() const {
    return _username;
}

bool Client::isRegistered() const {
	if (_nickOK == true && _userOK == true) {
		return true;
	} else {
		return false;
	}
}

void Client::setUnRegistered() {
	_registered = false;
	_nickOK = false;
	_userOK = false;
}

bool Client::getAuthenticated() const {
	return _authenticated;
}

void Client::setAutenticated(bool authenticated) {
	_authenticated = authenticated;
}

void Client::setNickOK() {
	_nickOK = true;
}

void Client::setUserOK() {
	_userOK = true;
}

void Client::registerUser() {
    _registered = true;
}

void Client::clear() {
    if (_fd > 0) {
        close(_fd);
    }
    _fd = -1;
    _nickname.clear();
    _username.clear();
    _registered = false;
	_authenticated = false;
	_nickOK = false;
	_userOK = false;
	_recvBuffer.clear();
}

void Client::sendMessage(const std::string& msg) const {
	if (_fd <= 0) return;
	
	ssize_t result = send(_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
	if (result == -1) {
		std::cerr << "Error sending message to client " << _nickname << std::endl;
	}
}

void Client::sendReply(const std::string& code, const std::string& message)
{
    std::string nickname = _nickname.empty() ? "*" : _nickname;
    std::string reply = ":irc " + code + " " + nickname + " " + message + "\r\n";
    sendMessage(reply);
}

std::string& Client::getRecvBuffer() {
	return _recvBuffer;
}

void Client::clearRecvBuffer() {
	_recvBuffer.clear();
}
