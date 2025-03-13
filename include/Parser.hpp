#ifndef PARSER_HPP
# define PARSER_HPP

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>

#include "Client.hpp"
#include "Log.hpp"
#include "Cmd.hpp"

# define RED		"\x1b[31m"
# define GREEN		"\x1b[32m"
# define YELLOW		"\x1b[33m"
# define BLUE		"\x1b[34m"
# define NC		"\033[0m"

#define MAX_ARGS 10

class Client;

class	Parser
{
	private:
		std::string					prefix_;
		std::string					command_;
		std::vector<std::string>	params_;
		int							n_params_;

		enum	CommandType
		{
			CMD_PASS,
			CMD_JOIN,
			//CMD_PRIVMSG,
			CMD_UNKNOWN
		};

	public:
		Parser();
		static void	Request(Client& client, const std::string& str); // se llama cuando se envía un mensaje

	private:
		static void	trimString(std::string& str);
		bool		parse(const std::string& request);
		void		parseParams(std::string args);
		CommandType	getCommandType() const;
		
		// bool	validatePrefix(int client_ID, bool& closed);
		// bool	validateArgs(int client_ID, bool& closed);
		bool	validateCommand();
		void	handleRequest(Client& client);
};

#endif
