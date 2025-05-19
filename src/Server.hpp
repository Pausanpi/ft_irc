/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:46:39 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/19 17:49:10 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include <string>
#include <cstdlib>
#include <cstdio>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#define PORT 6667

class Server {
public:
    Server();
    ~Server();

    void run(); // Main loop

private:
    int _serverFd;
    Client _clients[MAX_CLIENTS];

    void initSocket();
    void acceptNewClient();
    void handleClientData(int index);
    void removeClient(int index);
    void handleInput(Client &client, const std::string &input);
};

#endif