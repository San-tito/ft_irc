#include "Parser.hpp"

void Request(Client& client, const std::string& request)
{
	if (client.getFd() < 0 || request.empty())
	{
		Log::Err() << "Error: invalid connection index or empty request.";
		return;
	}

	Parser parser;
	if (!parser.parse(request))
	{
		Log::Err() << "Error parsing request from client " << client.getFd();
		return;
	}

	bool closed = false;
	if (!parser.validatePrefix(client.getFd(), closed) ||
		!parser.validateCommand(client.getFd(), closed) ||
		!parser.validateArgs(client.getFd(), closed))
	{
		Log::Err() << "Request validation failed for client " << client.getFd();
		return;
	}

	parser.handleRequest(client.getFd());
}


/**
 * Método que analiza la solicitud IRC y extrae su estructura.
 */
bool	Parser::parse(const std::string& request)
{
	std::string	tempRequest = request;
	trimString(tempRequest);

	size_t spacePos = tempRequest.find(' ');
	
	if (tempRequest[0] == ':')
	{
		if (spacePos == std::string::npos)
		{
			std::cerr << YELLOW"WARNING: command without prefix." << std::endl;
			return (false);
		}
		prefix_ = tempRequest.substr(1, spacePos - 1);
		tempRequest = tempRequest.substr(spacePos + 1);
	}

	spacePos = tempRequest.find(' ');
	if (spacePos != std::string::npos)
	{
		command_ = tempRequest.substr(0, spacePos);
		tempRequest = tempRequest.substr(spacePos + 1);
	} else {
		command_ = tempRequest;
		return (true);
	}

	parseArguments(tempRequest);
	return (true);
}
