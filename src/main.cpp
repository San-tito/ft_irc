/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/12 20:05:36 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int	main(int argc, char **argv)
{
	Server *server(new Server(argc, argv));

	// server->Run();

	
	std::vector<std::string> param;

	param.push_back("un parametro");
	param.push_back("dos parametros");
	param.push_back("tres parametros");

	// std::map<std::string, void(Cmd::*)(int, std::vector<std::string>)>::iterator it = Cmd::commands.find("JOIN");  
	// (c.*(it->second))(42, param); // client_fd, params&
	Cmd::Init();

	Cmd::commands["JOIN"](42, param);
	Cmd::commands["PASS"](42, param);
	
	delete (server);
	return (EXIT_SUCCESS);
}
