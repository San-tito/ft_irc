#include "Cmd.hpp"

std::map<std::string, void (*)(Client&, std::vector<std::string>)> Cmd::commands;

void Cmd::Init(void)
{
	commands["PASS"] = Pass;
	commands["NICK"] = Nick;
	commands["USER"] = User;
}

void Cmd::Pass(Client& client, std::vector<std::string> params)
{
	typedef std::vector<std::string>::iterator iterator;

	iterator user_pass(params.begin());

	if (params.size() != 1)
	{
		client << "PASS: Syntax Error";
		return ;
	}
	if (client.getPassword().empty())
	{
		client.setPassword(*user_pass);
		Log::Info() << "got PASS command";
	}
	else
		Log::Info() << "Password already registered";
}

void Cmd::Nick(Client& client, std::vector<std::string> params)
{
	typedef std::vector<std::string>::iterator iterator;

	iterator user_nick(params.begin());
	std::vector<Client> clients = Server::getClients();
	
	if (params.size() != 1)
	{
		client << "NICK: Syntax Error";
		return ;
	}
	if (user_nick->size() >= 9)
	{
		client << *user_nick << " :Nickname too long, max. 9 characters";
		return ;
	}
	//falta testear esto en principio si dos clientes se conectan con el mismo nickname deberia fallar.
	for (std::vector<Client>::iterator it(clients.begin()); it != clients.end(); it++)
	{
		if (!user_nick->compare(it->getNickname()))
		{
			client << *user_nick << " :Nickname already in use";
			return ;
		}
	}
	client.setNickname(*user_nick);
	Log::Info() << "got valid NICK command";
}

void Cmd::User(Client& client, std::vector<std::string> params)
{
	typedef std::vector<std::string>::iterator iterator;

	iterator user(params.begin());
	
	if (params.size() < 4)
	{
		client << "USER: Syntax Error";
		return ;
	}
	
	// check the 2 and 3 parameter it should be just one byte

	//it should be USER deo 0 * : tuta

	client.setUser(*user);
	Log::Info() << "got valid USER command";
}
