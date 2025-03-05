/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/05 10:44:49 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

sig_atomic_t IRCd::lastsignal_(0);

IRCd::IRCd(int argc, char **argv)
{
	ParseOptions(argc, argv);
	IoLibraryInit(CONNECTION_POOL);
	InitListener(port_, LISTEN_ADDR);
}

IRCd::~IRCd(void)
{
	std::cout << "Destructor called\n";
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

void IRCd::ParseOptions(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
		Exit(EXIT_FAILURE);
	}
	port_ = ParsePort(argv[1]);
	password_ = argv[2];
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

void IRCd::InitListener(unsigned short port, const char *listen_addr)
{
	ipaddr_t	addr;

	int sock, af;
	addr->sin4.sin_family = AF_INET;
	af = addr->sin4.sin_family;
	sock = socket(af, SOCK_STREAM, 0);
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
	std::cerr << "Listening on " << addr << ":" << port << ".\n";
}
