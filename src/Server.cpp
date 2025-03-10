/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/10 18:54:36 by rbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(int argc, char **argv) : socket_(-1)
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
	this->socket_ = Conn::NewListener(LISTEN_ADDR, this->port_);
	if (this->socket_ < 0)
		Exit(EXIT_FAILURE);
	AddPoll(this->socket_);
}

Server::~Server(void)
{
	if (this->socket_ >= 0)
	{
		close(this->socket_);
		Log::Info() << "Listening socket " << this->socket_ << " closed.\n";
	}
	Sig::Exit();
}

void Server::AddPoll(int fd)
{
	pollfd	poll;

	poll.fd = fd;
	poll.events = POLLIN | POLLPRI;
	this->pollfds_.push_back(poll);
}

void Server::NewConnection(void)
{
	int client_socket(accept(this->socket_, 0, 0));
	if (client_socket < 0)
	{
		Log::Err() << "Error en accept";
		Exit(EXIT_FAILURE);
	}
	Log::Info() << "Nueva conexión aceptada: fd " << client_socket;
	AddPoll(client_socket);
	clients_.push_back(Client(client_socket));
}

void Server::DeleteClient(int fd) {
	for (size_t i = 0; i < this->pollfds_.size(); i++){
		if (pollfds_[i].fd == fd){
			close(pollfds_[i].fd);
			pollfds_.erase(pollfds_.begin() + i);
			break;
		}
	}
	for (size_t i = 0; i < this->clients_.size(); i++) {
		if (clients_[i].getFd() == fd) {
			clients_.erase(clients_.begin() + i);
			break;
		}
	}
}

void Server::TimeOutCheck(void) {
	for (size_t j = 0; j < clients_.size(); j++)
	{
		if (time(NULL) > clients_[j].getTime() + 5) {
			Log::Info() << "Timeout: Client " << clients_[j].getFd() << " unregistered.";
			DeleteClient(clients_[j].getFd());
		}
	}
}

void Server::Run(void)
{
	while (!Sig::quit)
	{
		TimeOutCheck();
		int ret(poll(pollfds_.data(), pollfds_.size(), 1000));
		if (ret < 0)
			break;
		for (size_t i = 0; i < this->pollfds_.size(); i++)
		{
			if (pollfds_[i].revents & (POLLIN | POLLPRI))
			{
				if (pollfds_[i].fd == this->socket_)
					NewConnection();
				else {
					if (readRequest(pollfds_[i].fd) == -1){
						close(pollfds_[i].fd);
						pollfds_.erase(pollfds_.begin() + i);
						--i;
					}
				}
			}
		}
	}
	Log::Info() << "Server going down NOW!";
}

int Server::readRequest(int fd) {
	char buffer[1024];

	int bytes_read = recv(fd, buffer, sizeof(buffer) - 1, 0);
	if (bytes_read <= 0) { 
		if (bytes_read == 0)
			Log::Info() << "Cliente desconectado: fd " << fd ;
		else
			Log::Err() << "Error en recv: " << strerror(errno);
		return (-1);
	} else {
		buffer[bytes_read] = '\0';
		Log::Info() << "Mensaje recibido de fd " << fd << ": " << buffer;
		if (send(fd, buffer, bytes_read, 0) < 0)
			Log::Err() << "Error en send: " << strerror(errno);
	}
	return (0);
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
