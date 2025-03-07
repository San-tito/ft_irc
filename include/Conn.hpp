/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Conn.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/07 16:25:09 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONN_HPP
# define CONN_HPP

# include <arpa/inet.h>
# include <cerrno>
# include <cstdlib>
# include <cstring>
# include <fcntl.h>
# include <iostream>
# include <netinet/in.h>
# include <poll.h>
# include <sys/socket.h>
# include <unistd.h>

class Conn
{
  public:
	static int NewListener(const char *listen_addr, unsigned short port);
};

#endif /* CONN_HPP */
