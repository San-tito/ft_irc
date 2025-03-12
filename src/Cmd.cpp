#include "Cmd.hpp"

std::map<std::string, void (*)(int, std::vector<std::string>)> Cmd::commands;

void Cmd::Init(void)
{
	commands["JOIN"] = Join;
}

void Cmd::Join(int client_fd, std::vector<std::string> params)
{
	typedef std::vector<std::string>::iterator iterator;
	std::cout << "En efecto yo soy el " << client_fd << " y tengo estos parametros:\n";
	for (iterator it(params.begin()); it != params.end(); it++)
		std::cout << *it << "\n";
	std::cout << "\n";
}
