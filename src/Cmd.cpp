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
		Log::Err() << "PASS: Syntax Error";
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

	//it remains to check if the Nick given its unique
	
	if (params.size() != 1)
	{
		Log::Err() << "NICK: Syntax Error";
		return ;
	}
	if (user_nick->size() >= 9)
	{
		client << *user_nick << " :Nickname too long, max. 9 characters";
		return ;
	}
	client.setNickname(*user_nick);
	Log::Info() << "got valid NICK command";
}

void Cmd::User(Client& client, std::vector<std::string> params)
{
	typedef std::vector<std::string>::iterator iterator;

	iterator user(params.begin());
	
	//it should be USER deo 0 * : tuta
	// but u can pass even this USER deo a a :real
	// and the realname may contain spaces

	client.setUser(*user);
	Log::Info() << "got valid USER command";
}
