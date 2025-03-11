#include "Client.hpp"

Client::Client(void) : user_(""), last_time_(time(0)), registered_(false)
{
	poll_.fd = -1;
	poll_.events = 0;
	poll_.revents = 0;
}

Client::~Client(void)
{
}

int Client::getFd(void) const
{
	return (this->poll_.fd);
}

void Client::setFd(int fd)
{
	this->poll_.fd = fd;
}

short Client::getEvents(void) const
{
	return (this->poll_.events);
}

void Client::setEvents(short events)
{
	this->poll_.events |= events;
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
