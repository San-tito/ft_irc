#include "Cmd.hpp"

std::map<std::string, void (*)(Client *,
	std::vector<std::string>)> Cmd::commands;

void Cmd::Init(void)
{
	commands["JOIN"] = Join;
	commands["NICK"] = Nick;
	commands["PASS"] = Pass;
	commands["PART"] = Part;
	commands["PRIVMSG"] = Privmsg;
	commands["USER"] = User;
	commands["QUIT"] = Quit;
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
	bool moderated(false);
	std::string channame;
	std::stringstream chan_ss(params[0]);
	while (std::getline(chan_ss, channame, ','))
	{
		Channel *chan(Channel::Search(channame));
		Membership *is_member(chan ? Membership::Get(client, chan) : 0);
		if (is_member)
			continue ;
		if (chan)
		{
			// if (!JoinAllowed(client, chan))
			// 	continue ;
		}
		if (!chan && channame[0] != '+')
			moderated = true;
		if (!Channel::Join(client, channame))
			continue ;
		if (!chan)
		{
			chan = Channel::Search(channame);
			is_member = Membership::Get(client, chan);
		}
		if (moderated)
			is_member->AddMode('o');
		/* tell users in this channel about the new client */
		(*client) << "JOIN: " << channame << '\n';
	}
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
	if (!ValidateParams(client, 2, 2, params.size()))
		return ;
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
		client->setNick(params[0]);
		(*client) << "NICK: " << client->getNick() << '\n';
	}
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
