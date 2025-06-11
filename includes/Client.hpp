/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:39:46 by pausanch          #+#    #+#             */
/*   Updated: 2025/06/11 16:09:15 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <unistd.h>
#include <sys/socket.h>

class Client {
public:
    Client();
    ~Client();

    void setFd(int fd);
    int getFd() const;

    void setNickname(const std::string& nick);
    const std::string& getNickname() const;

    void setUsername(const std::string& user);
    const std::string& getUsername() const;

    bool isRegistered() const;
    void registerUser();

    void clear();
    void sendMessage(const std::string& msg) const;

	bool getAuthenticated() const;
	void setAutenticated(bool authenticated);
	void setNickOK();
	void setUserOK();
	
	std::string& getRecvBuffer();
	void clearRecvBuffer();

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    bool _registered;
	bool _authenticated;
	bool _nickOK;
	bool _userOK;
	std::string _recvBuffer;
};

#endif