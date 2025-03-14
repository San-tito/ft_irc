#ifndef CMD_HPP
# define CMD_HPP

# include "Client.hpp"
# include <iostream>
# include <map>
# include <string>
# include <vector>

class Cmd
{
  public:
	static void Init(void);
	static std::map<std::string, void (*)(Client &,
		std::vector<std::string>)> commands;

  private:
	static bool ValidateRegister(Client &client);
	static bool ValidateParams(Client &client, int min, int max, int argc);
	static void Join(Client &client, std::vector<std::string> params);
	static void Pass(Client &client, std::vector<std::string> params);
};

#endif
