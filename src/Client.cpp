/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jsalado- <jsalado-@student.42malaga.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:40:17 by pausanch          #+#    #+#             */
/*   Updated: 2025/06/16 13:04:01 by jsalado-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _fd(0), _registered(false), _authenticated(false) {}

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
	return _nickOK && _userOK;
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
    close(_fd);
    _fd = 0;
    _nickname.clear();
    _username.clear();
    _registered = false;
	_authenticated = false;
}

void Client::sendMessage(const std::string& msg) const {
	send(_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL);
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
