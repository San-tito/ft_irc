#include "Client.hpp"

Client::Client(int fd, struct sockaddr_in addr) : fd_(fd), addr_(addr),
	nick_(""), user_(""), log_time_(time(NULL)), is_logged_(false)
{
}

Client::~Client(void)
{
}

int Client::getFd(void) const
{
	return (this->fd_);
}

int Client::getTime(void) const
{
	return (this->log_time_);
}
