/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/08 12:25:21 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

IRCd::IRCd(int argc, char **argv) : socket_(-1)
{
	if (argc != 3)
	{
		Log::Notice() << "Usage: " << argv[0] << " <port> <password>";
		Exit(EXIT_FAILURE);
	}
	this->port_ = ParsePort(argv[1]);
	this->password_ = argv[2];
	Log::Info() << "IRCd starting ...";
	Io::Init(CONNECTION_POOL);
	Sig::Init();
	this->socket_ = Conn::NewListener(LISTEN_ADDR, this->port_);
	if (this->socket_ < 0)
		Exit(EXIT_FAILURE);
}

IRCd::~IRCd(void)
{
	if (this->socket_ >= 0)
	{
		close(this->socket_);
		Log::Info() << "Listening socket " << this->socket_ << " closed.\n";
	}
	Sig::Exit();
}

void IRCd::Run(void)
{
	int				i;
	struct timeval	tv;

	while (!Sig::quit)
	{
		tv.tv_usec = 0;
		tv.tv_sec = 1;
		i = Io::Dispatch(&tv);
		if (i == -1 && errno != EINTR)
		{
			Log::Err() << "Io::Dispatch(): " << strerror(errno);
			Exit(EXIT_FAILURE);
		}
	}
	Log::Info() << "Server going down NOW!";
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
		Log::Notice() << "illegal port number " << arg << '!';
		Exit(EXIT_FAILURE);
	}
	return (0);
}
