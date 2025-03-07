/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 16:38:54 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

IRCd::IRCd(int argc, char **argv) : socket_(-1)
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>\n";
		Exit(EXIT_FAILURE);
	}
	this->port_ = ParsePort(argv[1]);
	this->password_ = argv[2];
	Io::Init(CONNECTION_POOL);
	Sig::Init();
	this->socket_ = Conn::NewListener(LISTEN_ADDR, this->port_);
}

IRCd::~IRCd(void)
{
	if (this->socket_ >= 0)
	{
		close(this->socket_);
		std::cout << "Listening socket " << this->socket_ << " closed.\n";
	}
	Sig::Exit();
}

void IRCd::Run(void)
{
	while (!Sig::quit)
	{
		// tuqui
	}
	std::cout << "Server going down NOW!\n";
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
