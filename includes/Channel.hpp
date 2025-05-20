/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:34:15 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/20 16:55:17 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <set>
#include <vector>

class Client; // forward declaration

class Channel {
private:
    std::string _name;
    std::set<Client*> _members;
	std::set<Client*> _operators;

public:
	Channel();
    Channel(const std::string& name);
    const std::string& getName() const;

    void addMember(Client* client);
    void removeMember(Client* client);
    const std::set<Client*>& getMembers() const;

	void addOperator(Client* client);
	void removeOperator(Client* client);
	bool isOperator(Client* client) const;
	void broadcast(const std::string &msg);
};

#endif
