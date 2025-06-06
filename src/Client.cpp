/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:40:17 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/27 13:11:24 by pausanch         ###   ########.fr       */
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

bool Client::getAuthenticated() const {
	return _authenticated;
}

void Client::setAutenticated(bool authenticated) {
	_authenticated = authenticated;
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
    send(_fd, msg.c_str(), msg.length(), 0);
}
