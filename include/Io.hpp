/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Io.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 16:32:19 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_HPP
# define IO_HPP

# include <iostream>
# include <poll.h>
# include <string>
# include <vector>

class Io
{
  public:
	static void Init(unsigned int eventsize);

  private:
	static unsigned int poll_maxfd;
	static std::vector<struct pollfd> pollfds;
};

#endif /* IO_HPP */
