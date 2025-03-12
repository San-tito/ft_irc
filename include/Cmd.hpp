#pragma once

# include "Client.hpp"
#include <iostream>
#include <map>
#include <vector>

class Cmd 
{
	private:
		void	Join(int client_fd, std::vector<std::string> name);
		// void	Nick(int client_fd, const std::string& name);
		// void	PrivMsg(int client_fd, const std::string& name);
		// void	User(int client_fd, const std::string& name);
	public:
		Cmd();
		static std::map<std::string, void(Cmd::*)(int, std::vector<std::string>)> commands;  
};
