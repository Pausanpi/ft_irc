/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:35:05 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/20 12:42:01 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Client.hpp"

Channel::Channel() : _name("default") {}

Channel::Channel(const std::string& name) : _name(name) {}

const std::string& Channel::getName() const {
    return _name;
}

void Channel::addMember(Client* client) {
    _members.insert(client);
}

void Channel::removeMember(Client* client) {
    _members.erase(client);
}

const std::set<Client*>& Channel::getMembers() const {
    return _members;
}
