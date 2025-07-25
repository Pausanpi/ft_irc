/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 12:34:15 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/15 11:59:34 by pausanch         ###   ########.fr       */
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
	std::set<Client*> _invited;
	bool _inviteOnly;
	std::string _modes;
	std::string _key;
	int	_limitmember;
	std::string _topic;
	bool _modeTopic;

public:
	Channel();
    Channel(const std::string& name);
    const std::string& getName() const;

    void addMember(Client* client);
    void removeMember(Client* client);
    const std::set<Client*>& getMembers() const;

	void addOperator(Client* client);
	void removeOperator(Client* client);
	const std::set<Client*>& getOperators() const;
	bool isOperator(Client* client) const;
	void broadcast(const std::string &msg);
	void broadcastToOthers(const std::string &msg, Client* excludeClient);

	void addInvited(Client* client);
	bool isInvited(Client* client) const;
	bool isInviteOnly() const;
	void removeInvited(Client* client);

	void addMode(char mode);
	void removeMode(char mode);
	bool hasMode(char mode) const;
	std::string getModes() const;

	void setKey(const std::string& key);
	void removeKey();
	const std::string& getKey() const;
	bool hasKey() const;

	void setlimit(int limit);
	int getlimit();
	void removelimit();
	int getnumberofmembers();

	std::string getUserList() const;
	
	void setTopic(const std::string& topic);
	const std::string& getTopic() const;
	bool hasModeTopic() const;
	void setModeTopic(bool mode);
	
};

#endif
