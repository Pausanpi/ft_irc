/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pausanch <pausanch@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:47:30 by pausanch          #+#    #+#             */
/*   Updated: 2025/05/27 12:36:38 by pausanch         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <stdexcept>
#include <exception>

int main(int argc, char **argv) {

	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
		return 1;
	}

	if (std::atoi(argv[1]) == 0 || std::atoi(argv[1]) < 1024 || std::atoi(argv[1]) > 65535)
	{
		std::cerr << "Error: Invalid port number. Must be between 1024 and 65535." << std::endl;
		return 1;
	}
	
	int port = std::atoi(argv[1]);
	std::string password = argv[2];
	
	try {
		Server server(port, password);
		server.run();
	} catch (const std::runtime_error& e) {
		std::cerr << "Runtime error: " << e.what() << std::endl;
		return 1;
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Unknown error occurred" << std::endl;
		return 1;
	}
	
	return 0;
}
