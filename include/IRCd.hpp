/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/10 16:48:12 by rbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCD_HPP
# define IRCD_HPP

# include "Conn.hpp"
# include "Log.hpp"
# include "Sig.hpp"
# include <algorithm>
# include <arpa/inet.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <sys/time.h>
# include <vector>

# define LISTEN_ADDR "0.0.0.0"

class IRCd
{
  public:
	IRCd(int argc, char **argv);
	~IRCd(void);
	void Run(void);
	void Exit(int status);

	void NewConnection(void);
	void AddPoll(int fd);

  private:
	unsigned short ParsePort(char *arg);

	int socket_;
	unsigned short port_;
	std::string password_;
	std::vector<struct pollfd> pollfds_;
};

#endif /* IRCD_HPP */
