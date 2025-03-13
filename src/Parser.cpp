#include "Parser.hpp"

Parser::Parser() : n_params_(0) { }

void Parser::Request(Client& client, const std::string& request)
{
	if (request.empty())
	{
		Log::Err() << "Error: empty request.";
		return ;
	}
	Parser parser;
	if (!parser.parse(request))
	{
		Log::Err() << "Error: parsing request from client " << client.getFd();
		return ;
	}

	if (!parser.validateCommand())
		return ;

	parser.handleRequest(client);
}

void	Parser::trimString(std::string& str)
{
	size_t start = str.find_first_not_of(" \t\r\n");
	size_t end = str.find_last_not_of(" \t\r\n");

	if (start == std::string::npos || end == std::string::npos)
		str.clear();
	else
		str = str.substr(start, end - start + 1);
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
			return (false);
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

	parseParams(tempRequest);
	return (true);
}

void Parser::parseParams(std::string args)
{
    trimString(args);
    size_t spacePos;

    while (!args.empty() && params_.size() < MAX_ARGS)
    {
        if (args[0] == ':')
        {
            // Si el parámetro comienza con ":", todo lo que sigue debe ser un solo parámetro
            params_.push_back(args.substr(1)); // Guarda todo el resto como un solo parámetro
            break;  // Salimos ya que el mensaje es el último parámetro
        }
        else
        {
            spacePos = args.find(' ');
            if (spacePos != std::string::npos)
            {
                params_.push_back(args.substr(0, spacePos)); // Agrega solo el parámetro actual
                args = args.substr(spacePos + 1);
                trimString(args); // Asegura que no queden espacios extra
            }
            else
            {
                params_.push_back(args);
                break; // Terminamos de agregar parámetros
            }
        }
    }
}


/**
 * Valida el comando usando `CommandType`.
 */
bool	Parser::validateCommand()
{
	CommandType cmdType = getCommandType();

	if (cmdType == CMD_UNKNOWN)
	{
		Log::Err() << RED"ERROR: Unknow command: '" << command_ << "'." << NC"";
		return (false);
	}
	return (true);
}

/**
 * Determina el tipo de comando para optimizar validaciones.
 */
Parser::CommandType	Parser::getCommandType() const
{
	if (command_ == "PASS") return (CMD_PASS);
	if (command_ == "JOIN") return (CMD_JOIN);
	//if (command_ == "PRIVMSG") return (CMD_PRIVMSG);
	return (CMD_UNKNOWN);
}

void	Parser::handleRequest(Client& client)
{

	Cmd::Init();
	Cmd::commands[command_](client, params_);
}
