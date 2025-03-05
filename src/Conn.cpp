/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conn.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/05 15:03:42 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Conn.hpp"

int Conn::NewListener(const char *listen_addr, unsigned short port)
{
	struct sockaddr_in	addr;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_aton(listen_addr, &addr.sin_addr) == 0)
	{
		std::cerr << "Can't listen on [" << listen_addr << "]:" << port << ": Failed to parse IP address!\n";
		return (-1);
	}
	addr.sin_port = htons(port);
	int af(addr.sin_family);
	int sock(socket(af, SOCK_STREAM, 0));
	if (sock < 0)
	{
		std::cerr << "Can't create socket (af " << af << ") : " << strerror(errno) << "!\n";
		return (-1);
	}
	if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
	{
		std::cerr << "Can't enable non-blocking mode for socket: " << strerror(errno) << "!\n";
		return (-1);
	}
	if (bind(sock, reinterpret_cast<struct sockaddr *>(&addr),
			sizeof(addr)) != 0)
	{
		std::cerr << "Can't bind socket to address " << listen_addr << ':';
		std::cerr << port << " - " << strerror(errno) << " !\n";
		close(sock);
		return (-1);
	}
	if (listen(sock, 10) != 0)
	{
		std::cerr << "Can't listen on socket: " << strerror(errno) << "!\n";
		close(sock);
		return (-1);
	}
	std::cout << "Now listening on [" << listen_addr << "]:" << port << " (socket " << sock << ").\n";
	return (sock);
}
