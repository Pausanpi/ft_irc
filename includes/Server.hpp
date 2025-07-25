/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:46:39 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/10 11:51:35 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <map>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

extern std::map<std::string, Channel> _channels;

class Server {
public:
    Server();
	Server(int port, const std::string &password);
    ~Server();

	int getPort() const;
	const std::string &getPassword() const;

    void run(); // Main loop
	void initSocket();
	void acceptNewClient();
	void handleClientData(int index);
	void handleInput(Client &client, const std::string &input);
	void removeClient(int index);

	
private:
	int _port;
	std::string _password;

    int _serverFd;
    Client _clients[MAX_CLIENTS];
};


#endif