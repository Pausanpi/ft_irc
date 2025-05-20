/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:41:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/20 13:23:33 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
std::map<std::string, Channel> _channels;

Server::Server() {
    initSocket();
}

Server::~Server() {
    close(_serverFd);
}

// Configura el servidor para que escuche conexiones entrantes
void Server::initSocket() {
    _serverFd = socket(AF_INET, SOCK_STREAM, 0);  // Crear socket TCP
    if (_serverFd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // Permitir reutilizar el puerto

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;  // Configuración de IPv4 y puerto
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_serverFd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(_serverFd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Server listening on port " << PORT << "..." << std::endl;
}


// Bucle principal del servidor
// Se encarga de usar select() para detectar eventos en los sockets, aceptar nuevos clientes
// y leer datos de los clientes existentes.
void Server::run() {
    fd_set read_fds;
    int max_fd;

    while (true) {
        FD_ZERO(&read_fds);
        FD_SET(_serverFd, &read_fds);
        max_fd = _serverFd;

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int fd = _clients[i].getFd();
            if (fd > 0) {
                FD_SET(fd, &read_fds);
                if (fd > max_fd)
                    max_fd = fd;
            }
        }

        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select");
            continue;
        }

        if (FD_ISSET(_serverFd, &read_fds))
            acceptNewClient();

        for (int i = 0; i < MAX_CLIENTS; ++i) {
            int fd = _clients[i].getFd();
            if (fd > 0 && FD_ISSET(fd, &read_fds)) {
                handleClientData(i);
            }
        }
    }
}

void Server::acceptNewClient() {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_fd = accept(_serverFd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("accept");
        return;
    }

    std::cout << "New connection: " << inet_ntoa(client_addr.sin_addr)
              << ":" << ntohs(client_addr.sin_port) << std::endl;

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (_clients[i].getFd() == 0) {
            _clients[i].setFd(client_fd);
            return;
        }
    }

    std::string msg = "Server full\r\n";
    send(client_fd, msg.c_str(), msg.length(), 0);
    close(client_fd);
}

void Server::handleClientData(int index) {
    char buffer[BUFFER_SIZE];
    int fd = _clients[index].getFd();
    int bytes = recv(fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes <= 0) {
        std::cout << "Client disconnected (fd " << fd << ")\n";
        removeClient(index);
    } else {
        buffer[bytes] = '\0';
        handleInput(_clients[index], buffer);
    }
}

void Server::removeClient(int index) {
    _clients[index].clear();
}

void Server::handleInput(Client &client, const std::string &input) {
    std::istringstream iss(input);
    std::string command;
    iss >> command;

    if (command == "NICK") {
        std::string nick;
        iss >> nick;
        client.setNickname(nick);
    } else if (command == "USER") {
        std::string user, a, b, real;
        iss >> user >> a >> b;
        std::getline(iss, real);
        client.setUsername(user);
        client.registerUser();

        std::ostringstream oss;
        oss << ":irc 001 " << client.getNickname()
            << " :Welcome to ft_irc, " << client.getNickname() << "!\r\n";
        client.sendMessage(oss.str());
    } else if (command == "PRIVMSG") {
		std::string target;
		iss >> target;

		std::string msg;
		std::getline(iss, msg);

		// Remove leading spaces
		size_t pos = msg.find_first_not_of(' ');
		if (pos != std::string::npos) {
			msg = msg.substr(pos);
		} else {
			msg.clear(); // no message content
		}

		// Remove leading ':' if present (common in IRC protocol)
		if (!msg.empty() && msg[0] == ':') {
			msg = msg.substr(1);
		}

		std::ostringstream oss;
		oss << ":" << client.getNickname()
			<< " PRIVMSG " << target << " :" << msg << "\r\n";
		std::string fullMsg = oss.str();

		if (!target.empty() && target[0] == '#') {
			// Message to channel
			std::map<std::string, Channel>::iterator it = _channels.find(target);
			if (it != _channels.end()) {
				Channel &channel = it->second;
				const std::set<Client *> &members = channel.getMembers();

				// Check if sender is in the channel members
				if (members.find(&client) == members.end()) {
					// Client is NOT in the channel, reject sending message
					// Optionally send an error message back to client:
					client.sendMessage("404 " + client.getNickname() + " " + target + " :Cannot send to channel (not a member)\r\n");
					return;
				}

				// Client is in the channel, send the message to all other members
				for (std::set<Client *>::const_iterator iter = members.begin(); iter != members.end(); ++iter) {
					Client *member = *iter;
					if (member->getFd() != client.getFd()) {
						member->sendMessage(fullMsg);
					}
				}
			} else {
				// Channel not found: optionally handle error
				client.sendMessage("403 " + client.getNickname() + " " + target + " :No such channel\r\n");
			}
		} else {
			// Message to user
			for (int i = 0; i < MAX_CLIENTS; ++i) {
				if (_clients[i].getFd() > 0 && _clients[i].getNickname() == target) {
					_clients[i].sendMessage(fullMsg);
					break;
				}
			}
		}

	} else if (command == "JOIN") {
        std::string chanName;
        iss >> chanName;

        if (chanName.empty()) {
            client.sendMessage(":irc 461 JOIN :Not enough parameters\r\n");
            return;
        }

        // Create channel if it doesn't exist
        if (_channels.find(chanName) == _channels.end()) {
            _channels.insert(std::make_pair(chanName, Channel(chanName)));
        }

        // Add client to the channel
        Channel& channel = _channels[chanName];
        channel.addMember(&client);

        std::ostringstream joinMsg;
        joinMsg << ":" << client.getNickname() << " JOIN " << chanName << "\r\n";
        client.sendMessage(joinMsg.str());
    } else if (command == "QUIT") { //esto hay que cambiarlo
        client.sendMessage("Goodbye!\r\n");
        client.clear();
    }
	else {
		std::cout << "Unknown command: " << command << std::endl;
	}
}

