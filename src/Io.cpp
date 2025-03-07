/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Io.cpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:57:02 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 16:56:45 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Io.hpp"

int Io::poll_maxfd;
std::vector<struct pollfd> Io::pollfds;

void Io::Init(unsigned int eventsize)
{
	pollfds.resize(eventsize);
	poll_maxfd = 0;
	std::cerr << "IO subsystem: poll (initial maxfd " << eventsize << ").\n";
	for (unsigned int i = 0; i < eventsize; ++i)
		pollfds[i].fd = -1;
}

int Io::Dispatch(struct timeval *tv)
{
	time_t sec(tv->tv_sec * 1000);
	int i, ret, timeout(tv->tv_usec + sec);
	int fds_ready(0);
	short what(0);
	if (timeout < 0)
		timeout = 1000;
	ret = poll(&pollfds[0], poll_maxfd + 1, timeout);
	if (ret <= 0)
		return (ret);
	fds_ready = ret;
	for (i = 0; i <= poll_maxfd; ++i)
	{
		what = 0;
		if (pollfds[i].revents & (POLLIN | POLLPRI))
			what = IO_WANTREAD;
		if (pollfds[i].revents & POLLOUT)
			what |= IO_WANTWRITE;
		if (pollfds[i].revents && !what)
			what |= IO_ERROR;
		if (what)
			fds_ready--;
		if (fds_ready <= 0)
			break ;
	}
	return (ret);
}
