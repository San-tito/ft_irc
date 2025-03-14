#include "Cmd.hpp"

std::map<std::string, void (*)(Client &,
	std::vector<std::string>)> Cmd::commands;

void Cmd::Init(void)
{
	commands["JOIN"] = Join;
	commands["PASS"] = Pass;
}

bool Cmd::ValidateRegister(Client &client)
{
	if (!client.isRegistered())
	{
		client << client.getFd() << " :Connection not registered\n";
		return (false);
	}
	return (true);
}

bool Cmd::ValidateParams(Client &client, int min, int max, int argc)
{
	if (argc < min || (max != -1 && argc > max))
	{
		client << client.getFd() << " :Syntax error\n";
		return (false);
	}
	return (true);
}

void Cmd::Join(Client &client, std::vector<std::string> params)
{
	if (!ValidateRegister(client))
		return ;
	if (!ValidateParams(client, 1, 2, params.size()))
		return ;
}

void Cmd::Pass(Client &client, std::vector<std::string> params)
{
	if (!ValidateParams(client, 0, -1, params.size()))
		return ;
}
