#include "Membership.hpp"

Membership::Membership(Client &client, Channel &channel) : client_(client),
	channel_(channel)
{
}

Membership::~Membership(void)
{
}

Client &Membership::getClient(void)
{
	return (client_);
}

Channel &Membership::getChannel(void)
{
	return (channel_);
}

void Membership::AddMode(char mode)
{
	modes_.insert(mode);
}

Membership *Membership::Get(Client &client, Channel &channel)
{
	std::vector<Membership>::iterator it(Server::memberships.begin());
	while (it != Server::memberships.end())
	{
		if (&it->getClient() == &client && &it->getChannel() == &channel)
			return (&(*it));
		++it;
	}
	return (0);
}
