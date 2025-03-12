#include "Client.hpp"

Client::Client(int fd, short events) : user_(""), last_time_(time(0)),
	registered_(false)
{
	poll_.fd = fd;
	poll_.events = events;
}

Client::~Client(void)
{
}

std::string Client::getReadBuffer(void) const
{
	return (this->rbuffer_);
}

std::string Client::getWriteBuffer(void) const
{
	return (this->wbuffer_);
}

void Client::setReadBuffer(std::string buffer)
{
	this->rbuffer_ = buffer;
}

void Client::setWriteBuffer(std::string buffer)
{
  this->wbuffer_ = buffer;
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

void Client::unsetEvent(short event)
{
	this->poll_.events &= ~event;
}

void Client::unsetReadBuffer(void)
{
	this->rbuffer_.clear();
}

void Client::unsetWriteBuffer(void)
{
	this->wbuffer_.clear();
}
