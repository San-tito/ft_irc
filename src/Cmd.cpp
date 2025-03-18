#include "Cmd.hpp"

std::map<std::string, void (*)(Client *,
	std::vector<std::string>)> Cmd::commands;

void Cmd::Init(void)
{
	commands["INVITE"] = Invite;
	commands["JOIN"] = Join;
	commands["KICK"] = Kick;
	commands["MODE"] = Mode;
	commands["NICK"] = Nick;
	commands["PART"] = Part;
	commands["PASS"] = Pass;
	commands["PRIVMSG"] = Privmsg;
	commands["QUIT"] = Quit;
	commands["TOPIC"] = Topic;
	commands["USER"] = User;
}

static bool	ValidateRegister(Client *client)
{
	if (!client->isRegistered())
	{
		(*client) << "Connection not registered\n";
		return (false);
	}
	return (true);
}

static bool	ValidateParams(Client *client, int min, int max, int argc)
{
	if (argc < min || (max != -1 && argc > max))
	{
		(*client) << "Syntax error\n";
		return (false);
	}
	return (true);
}

static bool	JoinAllowed(Client *client, Channel *chan, std::string key)
{
	bool is_invited(chan->IsInvited(client));
	if (chan->HasMode('i') && !is_invited)
	{
		(*client) << "Cannot join channel (+i) -- Invited users only\n";
		return (false);
	}
	if (chan->HasMode('k') && chan->getKey() != key)
	{
		(*client) << "Cannot join channel (+k) -- Wrong channel key\n";
		return (false);
	}
	if (chan->HasMode('l') && chan->getMaxUsers() <= Channel::MemberCount(chan))
	{
		(*client) << "Cannot join channel (+l) -- Channel is full, try later\n";
		return (false);
	}
	return (true);
}

static void	LoginUser(Client *client)
{
	if (!Server::password.empty() && client->getPassword() != Server::password)
	{
		Log::Err() << "User \"" << client->getUser() << "\" rejected (connection " << client->getFd() << "): Bad server password!";
		Client::Destroy(client);
		return ;
	}
	client->setRegistered(true);
	Log::Info() << "User \"" << client->getNick() << "\" registered (connection " << client->getFd() << ").";
	(*client) << client->getNick() << " :Welcome to the jungle!\n";
}

void Cmd::Invite(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 2, 2, params.size()))
		return ;
	Client *target(Client::Search(params[0]));
	if (!target)
	{
		(*client) << "No such nick or channel name\n";
		return ;
	}
	Channel *chan(Channel::Search(params[1]));
	if (chan)
	{
		Membership *member(Membership::Get(client, chan));
		if (!member)
		{
			(*client) << "You are not on that channel\n";
			return ;
		}
		if (chan->HasMode('i') && !member->HasMode('o'))
		{
			(*client) << "You are not channel operator\n";
			return ;
		}
		else
			chan->AddInvite(target);
	}
	Log::Info() << "User " << client->getNick() << " invites " << params[0] << " to " << params[1];
	(*target) << ":" << client->getNick() << " INVITE " << params[0] << " " << params[1] << '\n';
}

void Cmd::Join(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 1, 2, params.size()))
		return ;
	if (params[0].empty())
	{
		(*client) << "Syntax error\n";
		return ;
	}
	if (params.size() == 1 && params[0] == "0")
	{
		Channel::PartAll(client);
		return ;
	}
	bool op(false);
	std::string key;
	std::string channame;
	std::stringstream key_ss;
	std::stringstream chan_ss(params[0]);
	if (params.size() > 1)
	{
		key_ss.str(params[1]);
		key_ss.clear();
		std::getline(key_ss, key, ',');
	}
	while (std::getline(chan_ss, channame, ','))
	{
		Channel *chan(Channel::Search(channame));
		Membership *is_member(chan ? Membership::Get(client, chan) : 0);
		if (is_member)
			continue ;
		if (chan)
		{
			if (!JoinAllowed(client, chan, key))
				continue ;
		}
		if (!chan && channame[0] != '+')
			op = true;
		if (!Channel::Join(client, channame))
			continue ;
		if (!chan)
		{
			chan = Channel::Search(channame);
			is_member = Membership::Get(client, chan);
		}
		if (op)
			is_member->AddMode('o');
		chan->Write(client, std::string(":") + client->getNick() + " JOIN :"
			+ chan->getName() + '\n');
		(*client) << ":" << client->getNick() << " JOIN :" << chan->getName() << '\n';
		if (params.size() > 1)
			std::getline(key_ss, key, ',');
	}
}

void Cmd::Kick(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 2, 3, params.size()))
		return ;
	unsigned int channel_count(0);
	unsigned int nick_count(0);
	std::string item;
	std::stringstream ss(params[0]);
	while (std::getline(ss, item, ','))
		channel_count++;
	ss.str(params[1]);
	ss.clear();
	while (std::getline(ss, item, ','))
		nick_count++;
	std::string current_nick;
	std::string current_channel;
	std::string reason(params.size() == 3 ? params[2] : client->getNick());
	if (channel_count == 1)
	{
		ss.str(params[1]);
		ss.clear();
		while (std::getline(ss, current_nick, ','))
		{
			Channel::Kick(client, current_nick, params[0], reason);
			nick_count--;
		}
	}
	else if (channel_count == nick_count)
	{
		ss.str(params[0]);
		ss.clear();
		std::stringstream nick_ss(params[1]);
		nick_ss.clear();
		while (std::getline(ss, current_channel, ',') && std::getline(nick_ss,
				current_nick, ','))
		{
			Channel::Kick(client, current_nick, current_channel, reason);
			channel_count--;
			nick_count--;
		}
	}
	else
		(*client) << "Syntax error\n";
}

void Cmd::Mode(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 1, -1, params.size()))
		return ;
	Client *cl(0);
	Channel *chan(0);
	bool is_valid_nick(Client::IsValidNick(params[0]));
	bool is_valid_chan(Channel::IsValidName(params[0]));
	if (is_valid_nick)
		cl = Client::Search(params[0]);
	if (is_valid_chan)
		chan = Channel::Search(params[0]);
	if (cl)
		return (Client::Mode(client, params, cl));
	if (chan)
		return (Channel::Mode(client, params, chan));
	if (is_valid_nick)
		(*client) << "No such nick or channel name\n";
	else
		(*client) << "No such channel\n";
}

void Cmd::Part(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 1, 2, params.size()))
		return ;
	if (params[0].empty())
	{
		(*client) << "Syntax error\n";
		return ;
	}
	std::string chan;
	std::stringstream ss(params[0]);
	while (std::getline(ss, chan, ','))
		Channel::Part(client, chan, params.size() > 1 ? params[1] : "");
}

void Cmd::Quit(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 0, 1, params.size()))
		return ;
	if (params.size() == 1)
		Log::Info() << client->getNick() << ": " << params[0];
	Client::Destroy(client);
}

void Cmd::Privmsg(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (params.size() == 0)
	{
		(*client) << "No recipient given\n";
		return ;
	}
	else if (params.size() == 1)
	{
		(*client) << "No text to send\n";
		return ;
	}
	else if (!ValidateParams(client, 2, 2, params.size()))
		return ;
	else if (params[0].empty())
	{
		(*client) << "No text to send\n";
		return ;
	}
	std::string target;
	std::stringstream ss(params[0]);
	while (std::getline(ss, target, ','))
	{
		Client *dest(0);
		Channel *chan(0);
		if (target.find('!') != std::string::npos)
			dest = Client::Search(target);
		else
		{
			std::string nick(target.substr(0, target.find('!')));
			dest = Client::Search(nick);
		}
		if (dest)
			(*dest) << ":" << client->getNick() << " PRIVMSG " << target << " :" << params[1] << '\n';
		else if ((chan = Channel::Search(target)))
			chan->Write(client, std::string(":") + client->getNick()
				+ " PRIVMSG " + target + " :" + params[1]);
		else
		  client->WriteErr(ERR_NOSUCHNICK_MSG);
			//(*client) << ":" << client->getNick() << " "<< params[0] << " :No such nick or channel name\n";
	}
}

void Cmd::Pass(Client *client, std::vector<std::string> params)
{
	if (!ValidateParams(client, 1, 1, params.size()))
		return ;
	if (client->getPassword().empty())
	{
		Log::Info() << "Connection " << client->getFd() << ": got PASS command ...";
		client->setPassword(params[0]);
	}
	else
		(*client) << "Password already registered\n";
}

void Cmd::Nick(Client *client, std::vector<std::string> params)
{
	if (!ValidateParams(client, 1, 1, params.size()))
		return ;
	if (strcasecmp(client->getNick().c_str(), params[0].c_str()) != 0)
	{
		if (!Client::IsValidNick(params[0]))
		{
			if (params[0].size() > MAX_NICK_LEN)
				(*client) << "Nickname too long\n";
			else
				(*client) << "Erroneous nickname\n";
			return ;
		}
		if (Client::Search(params[0]))
		{
			(*client) << "Nickname already in use\n";
			return ;
		}
	}
	if (!client->isRegistered())
	{
		Log::Info() << "Connection " << client->getFd() << ": got valid NICK command ...";
		client->setNick(params[0]);
		if (!client->getUser().empty())
			return (LoginUser(client));
	}
	else
	{
		Log::Info() << "Connection " << client->getFd() << ": changed nickname to " << params[0];
		(*client) << ":" << client->getNick()<< " NICK :" << params[0] << "\n";
		client->Write(std::string(":") + client->getNick() + " NICK :" + params[0] + '\n');
		client->setNick(params[0]);
	}
}

void Cmd::Topic(Client *client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 1, 2, params.size()))
		return ;
	Channel *chan(Channel::Search(params[0]));
	if (!chan)
	{
		(*client) << "No such channel\n";
		return ;
	}
	Membership *member(Membership::Get(client, chan));
	if (!member)
	{
		(*client) << "You are not on that channel\n";
		return ;
	}
	if (params.size() == 1)
	{
		std::string topic(chan->getTopic());
		if (topic.empty())
		{
			(*client) << "No topic is set\n";
		}
		else
		{
			// (*client) << TOPIC_MSG // QA I NEED HELP
		}
		return ;
	}
	if (chan->HasMode('t') && !member->HasMode('o'))
	{
		(*client) << "You are not channel operator\n";
		return ;
	}
	// if(params[1] != chan->getTopic())
	// 	chan->Write(client, std::string(":") + client->getNick() + " TOPIC "
	// + chan->getName() + " :" + params[1] + '\n');
	chan->setTopic(params[1]);
	// client->Write(std::string(":") + client->getNick() + " TOPIC "
	// + chan->getName() + " :" + params[1] + '\n');
}

void Cmd::User(Client *client, std::vector<std::string> params)
{
	if (client->isRegistered())
	{
		(*client) << "Connection already registered\n";
		return ;
	}
	if (!ValidateParams(client, 4, 4, params.size()))
		return ;
	if (!client->getPassword().empty() || !client->getNick().empty())
	{
		if (params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-@._") != std::string::npos)
		{
			Client::Destroy(client);
			return ;
		}
		client->setUser(params[0]);
		Log::Info() << "Connection " << client->getFd() << ": got valid USER command ...";
		if (!client->getNick().empty())
			return (LoginUser(client));
	}
	else
		(*client) << "Connection not registered\n";
}
