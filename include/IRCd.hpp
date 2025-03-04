/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/04 17:14:56 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCD_HPP
# define IRCD_HPP

# include <algorithm>
# include <csignal>
# include <iostream>
# include <poll.h>
# include <sstream>
# include <string>
# include <vector>

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

	unsigned short port_;
	std::string password_;

	unsigned int poll_maxfd_;
	std::vector<struct pollfd> pollfds_;

	static sig_atomic_t lastsignal_;
};

#endif /* IRCD_HPP */
