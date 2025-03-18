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

void Channel::setMaxUsers(size_t max_users)
{
	max_users_ = max_users;
}

void Channel::setTopic(const std::string &topic)
{
	topic_ = topic;
}

void Channel::setKey(const std::string &key)
{
	key_ = key;
}

void Channel::AddMode(char mode)
{
	modes_.insert(mode);
}

bool Channel::HasMode(char mode) const
{
	return (modes_.find(mode) != modes_.end());
}

void Channel::DelMode(char mode)
{
	modes_.erase(mode);
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

void Channel::Write(Client *client, const std::string &message)
{
	std::vector<Membership *>::iterator it(Server::memberships.begin());
	while (it != Server::memberships.end())
	{
		if ((*it)->getChannel() == this && (*it)->getClient() != client)
			(*(*it)->getClient()) << message;
		++it;
	}
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

void Channel::Mode(Client *client, std::vector<std::string> &params,
	Channel *target)
{
	if (target->getName()[0] == '+')
	{
		(*client) << target->getName() << " :Channel doesn't support modes\n";
		return ;
	}
	if (params.size() <= 1)
	{
		std::set<char> modes(target->getModes());
		(*client) << target->getName() << " :Modes: ";
		for (std::set<char>::iterator it(modes.begin()); it != modes.end(); ++it)
			(*client) << *it;
		(*client) << "\n";
		return ;
	}
	Membership *member(Membership::Get(client, target));
	if (!member)
	{
		(*client) << target->getName() << " :You are not on that channel\n";
		return ;
	}
	bool is_op(member->HasMode('o'));
	for (size_t i = 1; i < params.size(); i++)
	{
		bool set(false);
		std::string mode(params[i]);
		if (mode[0] == '+')
		{
			set = true;
			mode = mode.substr(1);
		}
		else if (mode[0] == '-')
			mode = mode.substr(1);
		if (!is_op)
		{
			(*client) << target->getName() << " :You are not channel operator\n";
			break ;
		}
		switch (mode[0])
		{
		case 'i':
		case 't':
			set ? target->AddMode(mode[0]) : target->DelMode(mode[0]);
			break ;
		case 'k':
			if (!set)
			{
				target->DelMode('k');
				break ;
			}
			if (i + 1 >= params.size())
			{
				(*client) << "Syntax error\n";
				break ;
			}
			if (params[i + 1].empty() || params[i
				+ 1].find(' ') == std::string::npos)
			{
				(*client) << "Invalid mode parameter\n";
				break ;
			}
			target->DelMode('k');
			target->setKey(params[i + 1]);
			target->AddMode('k');
			break ;
		case 'l':
			if (!set)
			{
				target->DelMode('l');
				break ;
			}
			if (i + 1 >= params.size())
			{
				(*client) << "Syntax error\n";
				break ;
			}
			long l(std::atol(params[i + 1].c_str()));
			if (l <= 0 || l >= 0xFFFF)
			{
				(*client) << "Invalid mode parameter\n";
				break ;
			}
			target->DelMode('l');
			target->setMaxUsers(l);
			target->AddMode('l');
			break ;
		}
	}
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

void Channel::Kick(Client *client, const std::string &nick,
	const std::string &channel, const std::string &reason)
{
	Client *target(Client::Search(nick));
	if (!target)
	{
		(*client) << nick << " :No such nick or channel name\n";
		return ;
	}
	Channel *chan(Channel::Search(channel));
	if (!chan)
	{
		(*client) << channel << " :No such channel\n";
		return ;
	}
	Membership *member(Membership::Get(client, chan));
	if (!member)
	{
		(*client) << nick << " :You are not on that channel\n";
		return ;
	}
	Membership *target_member(Membership::Get(target, chan));
	if (!target_member)
	{
		(*client) << nick << " :They aren't on that channel\n";
		return ;
	}
	if (!member->HasMode('o'))
	{
		(*client) << nick << " :Your privileges are too low\n";
		return ;
	}
	Membership::Remove(target, chan);
	Log::Info() << "User " << nick << " was kicked from channel " << channel << " by " << client->getNick() << " (" << reason << ")\n";
}
