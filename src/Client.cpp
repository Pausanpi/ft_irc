/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:40:17 by pausanch          #+#    #+#             */
/*   Updated: 2025/06/11 16:09:08 by pausanch         ###   ########.fr       */
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
	if (_nickOK && _userOK) {
		return true;
	}
	return false;
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

/* //con comprabsion de errores
void Client::sendMessage(const std::string& msg) const {
    if (send(_fd, msg.c_str(), msg.length(), MSG_NOSIGNAL) < 0) {
        perror("send");
    }
} */

//el par de funciones para el buffer de ctrl+d
std::string& Client::getRecvBuffer() {
	return _recvBuffer;
}

void Client::clearRecvBuffer() { //a;adir ek ckear dek buffer al disconect
	_recvBuffer.clear();
}
