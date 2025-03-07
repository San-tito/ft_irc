/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Io.hpp                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 16:58:18 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IO_HPP
# define IO_HPP

# include <iostream>
# include <poll.h>
# include <string>
# include <sys/time.h>
# include <vector>

# define IO_WANTREAD 1
# define IO_WANTWRITE 2
# define IO_ERROR 4

class Io
{
  public:
	static void Init(unsigned int eventsize);
	static int Dispatch(struct timeval *tv);

  private:
	static int poll_maxfd;
	static std::vector<struct pollfd> pollfds;
};

#endif /* IO_HPP */
