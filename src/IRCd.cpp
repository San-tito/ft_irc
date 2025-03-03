/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/03 16:15:55 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "IRCd.hpp"

sig_atomic_t IRCd::lastsignal_(0);

IRCd::IRCd(int argc, char **argv)
{
	ParseOptions(argc, argv);
}

IRCd::~IRCd(void)
{
}

void IRCd::Run(void)
{
}

static unsigned short	port_parse(char *arg)
{
	int				port;
	unsigned short	port16;
	char			*endptr;

	port = strtol(arg, &endptr, 10);
	if (port > 0 && port < 0xFFFF && *endptr == '\0')
		port16 = port;
	else
	{
		std::cerr << "illegal port number " << arg << "!\n";
		std::exit(2);
	}
	return (port16);
}

void IRCd::ParseOptions(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "no broda\n";
		std::exit(2);
	}
	port_ = port_parse(argv[1]);
}
