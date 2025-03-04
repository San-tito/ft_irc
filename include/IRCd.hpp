/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/04 16:39:36 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCD_HPP
# define IRCD_HPP

# include <algorithm>
# include <csignal>
# include <iostream>
# include <sstream>
# include <string>

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

	unsigned short port_;
	std::string password_;
	static sig_atomic_t lastsignal_;
};

#endif /* IRCD_HPP */
