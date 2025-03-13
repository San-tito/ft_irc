#ifndef CMD_HPP
#define CMD_HPP

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include "Client.hpp"

class	Cmd
{
  private:
	static void Join(Client& client, std::vector<std::string> params);
	static void Pass(Client& client, std::vector<std::string> params);

  public:
	static void Init(void);
	static std::map<std::string, void (*)(Client&,
						std::vector<std::string>)>		commands;
};

#endif
