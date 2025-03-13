#ifndef CMD_HPP
#define CMD_HPP

#include "Server.hpp"
#include "Client.hpp"
#include <string>
#include <iostream>
#include <map>
#include <vector>

class Cmd
{
  private:
	static void Pass(Client& client, std::vector<std::string> params);
	static void Nick(Client& client, std::vector<std::string> params);
	static void User(Client& client, std::vector<std::string> params);
  public:
	static void Init(void);
	static std::map<std::string, void (*)(Client&,
		std::vector<std::string>)> commands;
};

#endif /* CMD_HPP */
