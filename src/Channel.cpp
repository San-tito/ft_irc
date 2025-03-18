#include "Channel.hpp"

Channel::Channel(const std::string &name) : name_(name), max_users_(0)
{
}

Channel::~Channel(void)
{
}

std::string Channel::getName(void) const
{
	return (name_);
}

std::set<char> Channel::getModes(void) const
{
	return (modes_);
}

std::string Channel::getTopic(void) const
{
	return (topic_);
}

std::string Channel::getKey(void) const
{
	return (key_);
}

size_t Channel::getMaxUsers(void) const
{
	return (max_users_);
}

void Channel::AddMode(char mode)
{
	modes_.insert(mode);
}

bool Channel::HasMode(char mode) const
{
	return (modes_.find(mode) != modes_.end());
}

void Channel::AddInvite(Client *client)
{
	invites_.push_back(client);
}

bool Channel::IsInvited(Client *client) const
{
	std::vector<Client *>::const_iterator it(invites_.begin());
	while (it != invites_.end())
	{
		if (*it == client)
			return (true);
		++it;
	}
	return (false);
}

void Channel::Exit(void)
{
	std::vector<Channel *>::iterator it(Server::channels.begin());
	while (it != Server::channels.end())
	{
		delete (*it);
		++it;
	}
}

bool Channel::IsValidName(const std::string &name)
{
	if (name.size() <= 1)
		return (false);
	if (name[0] != '#' && name[0] != '&' && name[0] != '+')
		return (false);
	if (name.size() > MAX_CHANNEL_LEN)
		return (false);
	return (true);
}

size_t Channel::MemberCount(const Channel *channel)
{
	size_t count(0);
	std::vector<Membership *>::iterator it(Server::memberships.begin());
	while (it != Server::memberships.end())
	{
		if ((*it)->getChannel() == channel)
			++count;
		++it;
	}
	return (count);
}

void Channel::Write(Client *client, const std::string &message)
{
	std::vector<Membership *>::iterator it(Server::memberships.begin());
	while (it != Server::memberships.end())
	{
		if ((*it)->getChannel() == (*it)->getChannel()
			&& (*it)->getClient() != client)
			(*(*it)->getClient()) << message;
		++it;
	}
}

Channel *Channel::Search(const std::string &name)
{
	std::vector<Channel *>::iterator it(Server::channels.begin());
	while (it != Server::channels.end())
	{
		if ((*it)->getName() == name)
			return (*it);
		++it;
	}
	return (0);
}

bool Channel::Join(Client *client, const std::string &name)
{
	if (!IsValidName(name))
	{
		(*client) << name << " :No such channel\n";
		return (false);
	}
	Channel *channel(Channel::Search(name));
	if (channel)
	{
		if (Membership::Get(client, channel))
			return (false);
	}
	else
	{
		channel = new Channel(name);
		Server::channels.push_back(channel);
	}
	Server::memberships.push_back(new Membership(client, channel));
	return (true);
}

void Channel::PartAll(Client *client)
{
	std::vector<Membership *>::iterator it(Server::memberships.begin());
	while (it != Server::memberships.end())
	{
		if ((*it)->getClient() == client)
		{
			Channel *channel((*it)->getChannel());
			Part(client, channel->getName(), "");
		}
		++it;
	}
}

void Channel::Part(Client *client, const std::string &name,
	const std::string &reason)
{
	Channel *channel(Channel::Search(name));
	if (!channel)
	{
		(*client) << name << " :No such channel\n";
		return ;
	}
	Membership *membership(Membership::Get(client, channel));
	if (membership == 0)
	{
		(*client) << name << " :You're not on that channel\n";
		return ;
	}
	Membership::Remove(client, channel);
	Log::Info() << "User " << client->getNick() << " left channel " << name << " (" << reason << ")\n";
}
