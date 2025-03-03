/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IRCd.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/03 16:24:07 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRCD_HPP
# define IRCD_HPP

# include <algorithm>
# include <csignal>
# include <iostream>
# include <sstream>
# include <string>

# define PASS_LEN 65

class IRCd
{
  public:
	IRCd(int argc, char **argv);
	~IRCd(void);
	void Run(void);

  private:
	std::string password_;
	unsigned short port_;
	static sig_atomic_t lastsignal_;
	void ParseOptions(int argc, char **argv);
};

#endif /* IRCD_HPP */
