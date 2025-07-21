/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:41:00 by pausanch          #+#    #+#             */
/*   Updated: 2025/07/10 12:03:08 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include "../includes/CommandHandler.hpp"
#include <algorithm>
#include <cctype>

std::map<std::string, Channel> _channels;

static Server *g_signal = NULL;

Server::Server()
{
	initSocket();
}

Server::Server(int port, const std::string &password) : _port(port), _password(password)
{
	initSocket();
}

Server::~Server()
{
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (_clients[i].getFd() > 0) {
			for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
				it->second.removeMember(&_clients[i]);
				it->second.removeOperator(&_clients[i]);
				it->second.removeInvited(&_clients[i]);
			}
			_clients[i].clear();
		}
	}
	if (_serverFd > 0) {
		close(_serverFd);
	}
}

int Server::getPort() const
{
	return _port;
}

const std::string &Server::getPassword() const
{
	return _password;
}

void Server::initSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

	int opt = 1;
	setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(_port);
	addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(_serverFd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	if (listen(_serverFd, 5) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	std::cout << "Server listening on port " << _port << "..." << std::endl;
}

void signalHandler(int signum)
{
	std::cout << "\nSignal " << signum << " received. Shutting down server." << std::endl;
	throw std::runtime_error("Server shutdown requested");
}

void Server::run()
{

	g_signal = this;

	std::signal(SIGINT, signalHandler);
	std::signal(SIGTERM, signalHandler);

	try
	{
		fd_set read_fds;
		int max_fd;
		while (true)
		{
			FD_ZERO(&read_fds);
			FD_SET(_serverFd, &read_fds);
			max_fd = _serverFd;

			for (int i = 0; i < MAX_CLIENTS; ++i)
			{
				int fd = _clients[i].getFd();
				if (fd > 0)
				{
					FD_SET(fd, &read_fds);
					if (fd > max_fd)
						max_fd = fd;
				}
			}

			if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) < 0)
			{
				perror("select");
				continue;
			}

			if (FD_ISSET(_serverFd, &read_fds))
				acceptNewClient();

			for (int i = 0; i < MAX_CLIENTS; ++i)
			{
				int fd = _clients[i].getFd();
				if (fd > 0 && FD_ISSET(fd, &read_fds))
				{
					handleClientData(i);
				}
			}
		}
	}
	catch (const std::exception &e)
	{
		std::cout << "Server shutting down: " << e.what() << std::endl;
	}
	g_signal = NULL;
}

void Server::acceptNewClient()
{
	struct sockaddr_in client_addr;
	socklen_t addr_len = sizeof(client_addr);
	int client_fd = accept(_serverFd, (struct sockaddr *)&client_addr, &addr_len);
	if (client_fd < 0)
	{
		perror("accept");
		return;
	}

	std::cout << "New connection: " << inet_ntoa(client_addr.sin_addr)
			  << ":" << ntohs(client_addr.sin_port) << std::endl;

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		if (_clients[i].getFd() == 0)
		{
			_clients[i].setFd(client_fd);
			return;
		}
	}

	std::string msg = "Server full\r\n";
	send(client_fd, msg.c_str(), msg.length(), 0);
	close(client_fd);
}

void Server::handleClientData(int index)
{
	char buffer[BUFFER_SIZE];
	int fd = _clients[index].getFd();

	int bytes = recv(fd, buffer, BUFFER_SIZE - 1, 0);

	if (bytes <= 0)
	{
		std::string reason = "Connection lost";
		std::istringstream quitStream("QUIT :" + reason);
		CommandHandler handler(_clients, _channels);
		handler.handleQUIT(_clients[index], quitStream);
	}
	else
	{
		buffer[bytes] = '\0';

		Client &client = _clients[index];
		client.getRecvBuffer().append(buffer);

		std::string::size_type pos;

		while ((pos = client.getRecvBuffer().find('\n')) != std::string::npos)
		{
			std::string line = client.getRecvBuffer().substr(0, pos);
			client.getRecvBuffer().erase(0, pos + 1);
			if (!line.empty() && line[line.size() - 1] == '\r')
				line.erase(line.size() - 1);
			std::cout << "Received line: [" << line << "]" << std::endl;
			handleInput(client, line);
		}
	}
}

void Server::handleInput(Client &client, const std::string &input)
{
	std::istringstream iss(input);
	std::string command;
	iss >> command;

	// Normalizar comando a mayúsculas
	std::transform(command.begin(), command.end(), command.begin(), ::toupper);

	CommandHandler handler(_clients, _channels);

	if (!client.getAuthenticated())
	{
		if (command == "PASS")
			handler.handlePASS(client, iss, _password);
		else
		{
			std::cout << "Client not authenticated. Command ignored: " << command << std::endl;
			client.sendReply("451", command + " :You have not registered");
		}
		return;
	}

	if (!client.isRegistered())
	{
		if (command == "NICK")
			handler.handleNICK(client, iss);
		else if (command == "USER")
			handler.handleUSER(client, iss);
		else if (command != "PASS")
		{
			std::cout << "Client not registered. Command ignored: " << command << std::endl;
			client.sendReply("451", command + " :You have not registered");
		}
		return;
	}

	if (command == "PRIVMSG")
		handler.handlePRIVMSG(client, iss);
	else if (command == "NICK")
		handler.handleChangeNICK(client, iss);
	else if (command == "JOIN")
		handler.handleJOIN(client, iss);
	else if (command == "QUIT")
		handler.handleQUIT(client, iss);
	else if (command == "MODE")
		handler.handleMODE(client, iss);
	else if (command == "KICK")
		handler.handleKICK(client, iss);
	else if (command == "INVITE")
		handler.handleINVITE(client, iss);
	else if (command == "TOPIC")
		handler.handleTOPIC(client, iss);
	else if (command != "PASS" && command != "NICK" && command != "USER")
	{
		std::cout << "Unknown command from registered user: " << command << std::endl;
		client.sendReply("421", (client.getNickname().empty() ? "*" : client.getNickname()) + " " + command + " :Unknown command");
	}
}

void Server::removeClient(int index) {
	if (index < 0 || index >= MAX_CLIENTS || _clients[index].getFd() <= 0) {
		return;
	}
	for (std::map<std::string, Channel>::iterator it = _channels.begin(); it != _channels.end(); ++it) {
		it->second.removeMember(&_clients[index]);
		it->second.removeOperator(&_clients[index]);
		it->second.removeInvited(&_clients[index]);
	}
	_clients[index].clear();
}
