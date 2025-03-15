#include "Cmd.hpp"

std::map<std::string, void (*)(Client &,
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

static bool	ValidateRegister(Client &client)
{
	if (!client.isRegistered())
	{
		client << "Connection not registered\n";
		return (false);
	}
	return (true);
}

static bool	ValidateParams(Client &client, int min, int max, int argc)
{
	if (argc < min || (max != -1 && argc > max))
	{
		client << "Syntax error\n";
		return (false);
	}
	return (true);
}

static Client	*ClientSearch(std::string nickname)
{
	std::vector<Client>::iterator it = Server::clients.begin();
	while (it != Server::clients.end())
	{
		if (strcmp(it->getNick().c_str(), nickname.c_str()) == 0)
			return (&(*it));
		it++;
	}
	return (NULL);
}

static bool	CheckNickname(Client &client, std::string nickname)
{
	if (nickname.size() > MAX_NICK_LEN)
	{
		client << "Nickname too long, max. " << MAX_NICK_LEN << " chars\n";
		return (false);
	}
	if (nickname.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789[]\\`_^{|}-") != std::string::npos)
	{
		client << "Erroneous nickname\n";
		return (false);
	}
	if (ClientSearch(nickname))
	{
		client << "Nickname already in use\n";
		return (false);
	}
	return (true);
}

static void	LoginUser(Client &client)
{
	if (!Server::password.empty() && strcmp(client.getPassword().c_str(),
			Server::password.c_str()) != 0)
	{
		Log::Err() << "User \"" << client.getUser() << "\" rejected (connection " << client.getFd() << "): Bad server password!";
		Server::CloseConnection(client.getFd());
		return ;
	}
	client.setRegistered(true);
	Log::Info() << "User \"" << client.getNick() << "\" registered (connection " << client.getFd() << ").";
	client << client.getNick() << " :Welcome to the jungle!\n";
}

void Cmd::Join(Client &client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 1, 2, params.size()))
		return ;

	// recibimos como maximo 2 parametros (channel y key)
	//JOIN #chan1                     //se une al canal 1 
	//JOIN #chan1,#chan2              //se une a los canales 1 y 2
	//JOIN &chan1 key1                //se une al canal 1 con la clave key1
	//JOIN #chan1,&chan2 key1         //se une al canales 1 con la clave key1 y chan2 no tiene key
	//JOIN #chan1,#chan2,#chan3 key1,key2,key3 //se une a los canales 1,2,3 con las claves key1,2,3
	//JOIN 0  					  //sale de todos los canales
}

void Cmd::Pass(Client &client, std::vector<std::string> params)
{
	if (!ValidateParams(client, 1, 1, params.size()))
		return ;
	if (client.getPassword().empty())
	{
		Log::Info() << "Connection " << client.getFd() << ": got PASS command ...";
		client.setPassword(params[0]);
	}
	else
		client << "Password already registered\n";
}

void Cmd::Nick(Client &client, std::vector<std::string> params)
{
	if (!ValidateParams(client, 1, 1, params.size()))
		return ;
	if (strcasecmp(client.getNick().c_str(), params[0].c_str()) != 0)
	{
		if (!CheckNickname(client, params[0]))
			return ;
	}
	if (!client.isRegistered())
	{
		Log::Info() << "Connection " << client.getFd() << ": got valid NICK command ...";
		client.setNick(params[0]);
		if (!client.getUser().empty())
			return (LoginUser(client));
	}
	else
	{
		Log::Info() << "Connection " << client.getFd() << ": changed nickname to " << params[0];
		client.setNick(params[0]);
		client << "NICK: " << client.getNick() << '\n';
	}
}

void Cmd::User(Client &client, std::vector<std::string> params)
{
	if (client.isRegistered())
	{
		client << "Connection already registered\n";
		return ;
	}
	if (!ValidateParams(client, 4, 4, params.size()))
		return ;
	if (!client.getPassword().empty() || !client.getNick().empty())
	{
		if (params[0].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789+-@._") != std::string::npos)
		{
			Server::CloseConnection(client.getFd());
			return ;
		}
		client.setUser(params[0]);
		Log::Info() << "Connection " << client.getFd() << ": got valid USER command ...";
		if (!client.getNick().empty())
			return (LoginUser(client));
	}
	else
		client << "Connection not registered\n";
}
