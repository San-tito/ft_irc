#include "Client.hpp"

Client::Client(void) : fd_(-1), user_(""), last_time_(time(0)),
	registered_(false)
{
}

Client::~Client(void)
{
	close(this->fd_);
}

int Client::getFd(void) const
{
	return (this->fd_);
}

void Client::setFd(int fd)
{
	this->fd_ = fd;
}

time_t Client::getLastTime(void) const
{
	return (this->last_time_);
}

void Client::setLastTime(time_t last_time)
{
	this->last_time_ = last_time;
}

bool Client::isRegistered(void) const
{
	return (this->registered_);
}

void Client::setRegistered(bool registered)
{
	this->registered_ = registered;
}
