/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/05 14:35:28 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

sig_atomic_t IRCd::lastsignal_(0);

IRCd::IRCd(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
		Exit(EXIT_FAILURE);
	}
	this->port_ = ParsePort(argv[1]);
	this->password_ = argv[2];
	IoLibraryInit(CONNECTION_POOL);
	this->socket_ = InitListener(this->port_, LISTEN_ADDR);
}

IRCd::~IRCd(void)
{
	close(this->socket_);
	std::cout << "Listening socket " << this->socket_ << " closed.\n";
}

void IRCd::Run(void)
{
}

void IRCd::Exit(int status)
{
	delete (this);
	std::exit(status);
}

unsigned short IRCd::ParsePort(char *arg)
{
	char	*endptr;

	int port(strtol(arg, &endptr, 10));
	if (port > 0 && port < 0xFFFF && *endptr == '\0')
		return (static_cast<unsigned short>(port));
	else
	{
		std::cerr << "illegal port number " << arg << "!\n";
		Exit(EXIT_FAILURE);
	}
	return (0);
}

void IRCd::IoLibraryInit(unsigned int eventsize)
{
	pollfds_.resize(eventsize);
	poll_maxfd_ = 0;
	std::cerr << "IO subsystem: poll (initial maxfd " << eventsize << ").\n";
	for (unsigned int i = 0; i < eventsize; ++i)
	{
		pollfds_[i].fd = -1;
	}
}

int IRCd::InitListener(unsigned short port, const char *listen_addr)
{
	struct sockaddr_in	addr;

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	if (inet_aton(listen_addr, &addr.sin_addr) == 0)
	{
		std::cerr << "Can't listen on [" << listen_addr << "]:" << port << ": Failed to parse IP address!\n";
		Exit(EXIT_FAILURE);
	}
	addr.sin_port = htons(port);
	int af(addr.sin_family);
	int sock(socket(af, SOCK_STREAM, 0));
	if (sock < 0)
	{
		std::cerr << "Can't create socket (af " << af << ") : " << strerror(errno) << "!\n";
		Exit(EXIT_FAILURE);
	}
	if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) < 0)
	{
		std::cerr << "Can't enable non-blocking mode for socket: " << strerror(errno) << "!\n";
		Exit(EXIT_FAILURE);
	}
	if (bind(sock, reinterpret_cast<struct sockaddr *>(&addr),
			sizeof(addr)) != 0)
	{
		std::cerr << "Can't bind socket to address " << listen_addr << ':';
		std::cerr << port << " - " << strerror(errno) << " !\n";
		close(sock);
		Exit(EXIT_FAILURE);
	}
	if (listen(sock, 10) != 0)
	{
		std::cerr << "Can't listen on socket: " << strerror(errno) << "!\n";
		close(sock);
		Exit(EXIT_FAILURE);
	}
	std::cout << "Now listening on [" << listen_addr << "]:" << port << " (socket " << sock << ").\n";
	return (sock);
}
