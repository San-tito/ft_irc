#include "Client.hpp"

Client::Client(int fd, struct sockaddr_in addr) : fd_(fd), addr_(addr),
	nick_(""), user_(""), last_time_(time(NULL)), registered_(false)
{
}

Client::~Client(void)
{
	// deberiamos poder hacer esto
	// close(this->fd_);
}

int Client::getFd(void) const
{
	return (this->fd_);
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
