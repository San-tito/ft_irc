#include "Cmd.hpp"

std::map<std::string, void (*)(Client&, std::vector<std::string>)> Cmd::commands;

void Cmd::Init(void)
{
	commands["JOIN"] = Join;
	commands["PASS"] = Pass;
}

void Cmd::Join(Client& client, std::vector<std::string> params)
{
	typedef std::vector<std::string>::iterator iterator;
	std::cout << "JOIN client[" << client.getFd() << "] y parametros: [";
	for (iterator it(params.begin()); it != params.end(); it++)
		std::cout << *it << ", ";
	std::cout << "]\n";
}

void Cmd::Pass(Client& client, std::vector<std::string> params)
{
	
	typedef std::vector<std::string>::iterator iterator;
	std::cout << "En efecto yo soy el pass " << client.getFd() << " y tengo estos parametros:\n";
	for (iterator it(params.begin()); it != params.end(); it++)
		std::cout << *it << "\n";
	std::cout << "\n";
}
