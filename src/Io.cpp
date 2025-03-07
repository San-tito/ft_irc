/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Io.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 16:34:17 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Io.hpp"

unsigned int Io::poll_maxfd;
std::vector<struct pollfd> Io::pollfds;

void Io::Init(unsigned int eventsize)
{
	pollfds.resize(eventsize);
	poll_maxfd = 0;
	std::cerr << "IO subsystem: poll (initial maxfd " << eventsize << ").\n";
	for (unsigned int i = 0; i < eventsize; ++i)
		pollfds[i].fd = -1;
}
