/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/10 16:59:17 by rbarbier         ###   ########.fr       */
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
	Sig::Init();
	this->socket_ = Conn::NewListener(LISTEN_ADDR, this->port_);
	if (this->socket_ < 0)
		Exit(EXIT_FAILURE);
	AddPoll(this->socket_);
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

void IRCd::AddPoll(int fd)
{
	pollfd	poll;

	poll.fd = fd;
	poll.events = POLLIN | POLLPRI;
	this->pollfds_.push_back(poll);
}

void IRCd::NewConnection(void)
{
	int client_socket(accept(this->socket_, 0, 0));
	if (client_socket < 0)
	{
		Log::Err() << "Error en accept";
		Exit(EXIT_FAILURE);
	}
	Log::Info() << "Nueva conexión aceptada: fd " << client_socket;
	AddPoll(client_socket);
}

void IRCd::Run(void)
{
	while (!Sig::quit)
	{
		int ret(poll(pollfds_.data(), pollfds_.size(), -1));
		if (ret < 0)
			return ;
		for (size_t i = 0; i < this->pollfds_.size(); i++)
		{
			if (pollfds_[i].revents & (POLLIN | POLLPRI))
			{
				if (pollfds_[i].fd == this->socket_)
					NewConnection();
				else
				{
					// clients[i].ReadRequest();
				}
			}
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
