/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/13 12:55:56 by ncastell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Parser.hpp"
#include "Server.hpp"
#include "Client.hpp"

#include <iostream>

int	main(int argc, char **argv)
{
	Server *server(new Server(argc, argv));

	// server->Run();

	 // Simulación de un cliente con un descriptor de archivo ficticio (por ejemplo, fd = 42)
    Client client(42, POLLIN);

    // Lista de solicitudes de prueba (sin `std::vector`, ya que es C++98)
    const int numTests = 6;
    std::string testRequests[numTests] = {
        ":user1 JOIN #channel",
        "PRIVMSG #channel :Hello, world!",
        "INVALIDCOMMAND arg1 arg2",
        ":user2 KICK #room user3",
        "INVITE user3 #room",
        ""
    };

    // Probar cada solicitud
    for (int i = 0; i < numTests; ++i)
    {
        std::cout << "\n--- Test " << (i + 1) << " ---\n";
        std::cout << "Request: " << testRequests[i] << "\n";

		Parser::Request(client, testRequests[i]); 
    }

	delete (server);
	return (EXIT_SUCCESS);
}
