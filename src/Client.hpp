/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:39:46 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/19 17:39:52 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>

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

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    bool _registered;
};

#endif