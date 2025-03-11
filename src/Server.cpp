/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/11 05:50:05 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int argc, char **argv) : sock_(-1)
{
	if (argc != 3)
	{
		Log::Notice() << "Usage: " << argv[0] << " <port> <password>";
		Exit(EXIT_FAILURE);
	}
	this->port_ = ParsePort(argv[1]);
	this->password_ = argv[2];
	Log::Info() << "Server starting ...";
	Sig::Init();
	this->sock_ = Conn::NewListener(LISTEN_ADDR, this->port_);
	if (this->sock_ < 0)
		Exit(EXIT_FAILURE);
	AddEvent(this->sock_, POLLIN | POLLPRI);
}

Server::~Server(void)
{
	if (this->sock_ >= 0)
	{
		close(this->sock_);
		Log::Info() << "Listening socket " << this->sock_ << " closed.\n";
	}
	Sig::Exit();
}

void Server::AddEvent(int fd, short events)
{
	pollfd	p;

	p.events = events;
	p.fd = fd;
	this->pollfds_.push_back(p);
}

void Server::NewConnection(void)
{
	int					new_sock;
	int					new_sock_len;
	struct sockaddr_in	new_addr;

	Log::Info() << "Accepting new connection on socket " << this->sock_ << "...";
	new_sock = accept(this->sock_,
			reinterpret_cast<struct sockaddr *>(&new_addr),
			reinterpret_cast<socklen_t *>(&new_sock_len));
	if (new_sock < 0)
	{
		Log::Err() << "Can't accept connection on socket " << this->sock_ << ": " << strerror(errno) << '!';
		Exit(EXIT_FAILURE);
	}
	if (!Conn::InitSocket(new_sock))
		return ;
	AddEvent(new_sock, POLLIN | POLLPRI);
	clients_.push_back(Client());
	clients_.back().setFd(new_sock);
	Log::Info() << "Accepted connection " << new_sock << " on socket " << this->sock_ << '.';
}

void Server::CloseConnection(int fd)
{
	Log::Info() << "Shutting down connection " << fd << " ...";
	for (size_t i = 0; i < this->pollfds_.size(); i++)
	{
		if (pollfds_[i].fd == fd)
		{
			pollfds_.erase(pollfds_.begin() + i);
			break ;
		}
	}
	for (size_t i = 0; i < this->clients_.size(); i++)
	{
		if (clients_[i].getFd() == fd)
		{
			clients_.erase(clients_.begin() + i);
			break ;
		}
	}
}

void Server::TimeOutCheck(void)
{
	time_t now(time(0));
	for (size_t i = 0; i < clients_.size(); i++)
	{
		if (clients_[i].isRegistered())
		{
			// handle pings pongs
		}
		else
		{
			if (clients_[i].getLastTime() < now - TIMEOUT)
			{
				Log::Info() << "Unregistered connection " << clients_[i].getFd() << " timed out ...";
				CloseConnection(clients_[i].getFd());
			}
		}
	}
}

void Server::Run(void)
{
	int	i;

	while (!Sig::quit)
	{
		TimeOutCheck();
		i = Dispatch();
		if (i == -1 && errno != EINTR)
		{
			Log::Err() << "Dispatch(): " << strerror(errno) << '!';
			Exit(1);
		}
	}
	Log::Info() << "Server going down NOW!";
}

int Server::Dispatch(void)
{
	int ret, fds_ready;
	ret = poll(this->pollfds_.data(), this->pollfds_.size(), 1000);
	if (ret <= 0)
		return (ret);
	fds_ready = ret;
	for (size_t i(0); i < this->pollfds_.size(); ++i)
	{
		pollfd p(pollfds_[i]);
		if (p.revents & (POLLIN | POLLPRI))
		{
			fds_ready--;
			if (p.fd == this->sock_)
				NewConnection();
			else
				ReadRequest(p.fd);
		}
		if (fds_ready <= 0)
			break ;
	}
	return (ret);
}

void Server::ReadRequest(int sock)
{
	ssize_t	len;
	char	readbuf[READBUFFER_LEN];

	len = read(sock, readbuf, sizeof(readbuf));
	if (len == 0)
	{
		CloseConnection(sock);
		return ;
	}
	if (len < 0)
	{
		if (errno == EAGAIN)
			return ;
		Log::Err() << "Read error on connection " << sock << ": " << strerror(errno) << '!';
		CloseConnection(sock);
		return ;
	}
	// if el cliente esta registrado
	// t = time(NULL);
	// clients_[i].setLastTime(t);
	// Log::Info() << "Mensaje recibido de fd " << fd << ": " << buffer;
	// // if (send(fd, buffer, bytes_read, 0) < 0)
	// // 	Log::Err() << "Error en send: " << strerror(errno);
}

void Server::Exit(int status)
{
	delete (this);
	std::exit(status);
}

unsigned short Server::ParsePort(char *arg)
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
