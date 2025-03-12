#ifndef CMD_HPP
#define CMD_HPP

#include "Client.hpp"
#include <iostream>
#include <map>
#include <vector>

class Cmd
{
  private:
	static void Join(int client_fd, std::vector<std::string> name);
	static void Pass(int client_fd, std::vector<std::string> name);
	// void	Nick(int client_fd, const std::string& name);
	// void	PrivMsg(int client_fd, const std::string& name);
	// void	User(int client_fd, const std::string& name);
  public:
	static void Init(void);
	static std::map<std::string, void (*)(int,
		std::vector<std::string>)> commands;
};


#endif /* CMD_HPP */
