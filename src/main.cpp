/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/04 17:15:09 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

int	main(int argc, char **argv)
{
	IRCd *server(new IRCd(argc, argv));
	server->Run();
	delete (server);
	return (EXIT_SUCCESS);
}
