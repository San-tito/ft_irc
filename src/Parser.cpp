#include "Parser.hpp"

bool	Request(Client& client, const std::string& request)
{
	if (client_ID < 0 || request.empty())
	{
		std::cerr << RED"Error: invalid conection index or empty request."<< NC""<< std::endl;
		return (false);
	}

	IRCParser parser;
	if (!parser.parse(request))
		return (false);

	bool	closed = false;
	/** VALIDACIONES:
		no se hasta que punto esto iria aqui*/
	if (!parser.validatePrefix(client_ID, closed) || !parser.validateCommand(client_ID, closed)
		|| !parser.validateArgs(client_ID, closed))
		return (!closed);

	return (parser.handleRequest(client_ID));
}

bool	



