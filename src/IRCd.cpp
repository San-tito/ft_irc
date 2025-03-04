/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/04 17:22:00 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

sig_atomic_t IRCd::lastsignal_(0);

IRCd::IRCd(int argc, char **argv)
{
	ParseOptions(argc, argv);
	IoLibraryInit(CONNECTION_POOL);
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
