/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:40:17 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/20 16:14:01 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client() : _fd(0), _registered(false) {}

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
    return _registered;
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
}

void Client::sendMessage(const std::string& msg) const {
    send(_fd, msg.c_str(), msg.length(), 0);
}
