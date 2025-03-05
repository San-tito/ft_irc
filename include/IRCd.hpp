/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/05 10:49:10 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCD_HPP
# define IRCD_HPP

# include <algorithm>
# include <csignal>
# include <cstdlib>
# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/socket.h>
# include <vector>

# define LISTEN_ADDR "0.0.0.0"
# define CONNECTION_POOL 100

class IRCd
{
  public:
	IRCd(int argc, char **argv);
	~IRCd(void);
	void Run(void);
	void Exit(int status);

  private:
	unsigned short ParsePort(char *arg);
	void ParseOptions(int argc, char **argv);
	void IoLibraryInit(unsigned int eventsize);
	void InitListener(unsigned short port, const char *listen_addr);

	unsigned short port_;
	std::string password_;

	unsigned int poll_maxfd_;
	std::vector<struct pollfd> pollfds_;

	static sig_atomic_t lastsignal_;
};

#endif /* IRCD_HPP */
