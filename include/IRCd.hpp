/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 22:31:52 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCD_HPP
# define IRCD_HPP

# include "Conn.hpp"
# include "Io.hpp"
# include "Sig.hpp"
# include <algorithm>
# include <arpa/inet.h>
# include <iostream>
# include <sstream>
# include <string>
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

	int socket_;
	unsigned short port_;
	std::string password_;
};

#endif /* IRCD_HPP */
