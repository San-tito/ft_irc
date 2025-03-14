/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/14 16:25:04 by ncastell         ###   ########.fr       */
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
	clients_.push_back(Client(this->sock_, POLLIN | POLLPRI));
	clients_.back().setRegistered(true);
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

void Server::NewConnection(int sock)
{
	int	new_sock;

	Log::Info() << "Accepting new connection on socket " << this->sock_ << "...";
	new_sock = accept(this->sock_, 0, 0);
	if (new_sock < 0)
	{
		Log::Err() << "Can't accept connection on socket " << this->sock_ << ": " << strerror(errno) << '!';
		Exit(EXIT_FAILURE);
	}
	if (!Conn::InitSocket(new_sock))
		return ;
	clients_.push_back(Client(new_sock, POLLIN | POLLPRI));
	Log::Info() << "Accepted connection " << new_sock << " on socket " << sock << '.';
}

void Server::CloseConnection(int fd)
{
	Log::Info() << "Shutting down connection " << fd << " ...";
	int i(getClient(fd));
	close(fd);
	clients_.erase(clients_.begin() + i);
}

void Server::TimeOutCheck(void)
{
	time_t now(time(0));
	for (size_t i = 0; i < clients_.size(); i++)
	{
			if (!clients_[i].isRegistered() && clients_[i].getLastTime() < now - TIMEOUT)
			{
				Log::Info() << "Unregistered connection " << clients_[i].getFd() << " timed out ...";
				CloseConnection(clients_[i].getFd());
			}
	}
}

void Server::ProcessRequest(Client &client)
{
	size_t pos(0);
	std::string str = client.getReadBuffer();
	if ((pos = str.find('\n')) == std::string::npos)
		return ;
	str = str.substr(0, pos);
	if (pos > 0 && str[pos - 1] == '\r')
		str = str.substr(0, pos - 1);
	if (str.size() > COMMAND_LEN)
	{
		Log::Err() << "Request too long (connection " << client.getFd() << "): " << str.size() << " bytes (max. " << COMMAND_LEN << " expected)!";
		CloseConnection(client.getFd());
		return ;
	}
	Log::Info() << "Received request from connection " << client.getFd() << ": " << str;
	// Parser::ParseRequest(client, str);
	client.unsetReadBuffer();
}

void Server::ProcessBuffers()
{
	for (size_t i = 0; i < clients_.size(); i++)
	{
		if (!clients_[i].getReadBuffer().empty())
			ProcessRequest(clients_[i]);
		if (!clients_[i].getWriteBuffer().empty())
			clients_[i].setEvents(POLLOUT);
	}
}

void Server::Run(void)
{
	int	i;

	i = 0;
	while (!Sig::quit)
	{
		TimeOutCheck();
		ProcessBuffers();
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
	std::vector<struct pollfd> pollfds(clients_.size());
	for (size_t i = 0; i < clients_.size(); ++i)
	{
		pollfds[i].fd = clients_[i].getFd();
		pollfds[i].events = clients_[i].getEvents();
	}
	ret = poll(pollfds.data(), pollfds.size(), 1000);
	if (ret <= 0)
		return (ret);
	fds_ready = ret;
	for (size_t i(0); i < pollfds.size(); ++i)
	{
		if (pollfds[i].revents & (POLLIN | POLLPRI | POLLOUT))
		{
			fds_ready--;
			if (pollfds[i].revents & (POLLIN | POLLPRI))
			{
				if (pollfds[i].fd == this->sock_)
					NewConnection(this->sock_);
				else
					ReadRequest(pollfds[i].fd);
			}
			if (pollfds[i].revents & POLLOUT)
				HandleWrite(pollfds[i].fd);
		}
		if (fds_ready <= 0)
			break ;
	}
	return (ret);
}

int Server::getClient(int fd)
{
	size_t	i;

	for (i = 0; i < clients_.size(); i++)
	{
		if (clients_[i].getFd() == fd)
			break ;
	}
	return (i);
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
	readbuf[len] = '\0';
	clients_[getClient(sock)].setReadBuffer(readbuf);
	Log::Info() << "Received " << len << " bytes from connection " << sock << ": " << readbuf;
}

void Server::HandleWrite(int sock)
{
	ssize_t len(0);
	int i(getClient(sock));
	len = write(sock, clients_[i].getWriteBuffer().c_str(),
			clients_[i].getWriteBuffer().size());
	clients_[i].unsetEvent(POLLOUT);
	clients_[i].unsetWriteBuffer();
	if (len < 0)
	{
		if (errno == EAGAIN || errno == EINTR)
			return ;
		Log::Err() << "Write error on connection " << sock << ": " << strerror(errno) << '!';
		CloseConnection(sock);
	}
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
