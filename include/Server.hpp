/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/10 18:54:58 by rbarbier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

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
# include "Client.hpp"

# define LISTEN_ADDR "0.0.0.0"

class Server
{
  public:
	Server(int argc, char **argv);
	~Server(void);
	void Run(void);
	void Exit(int status);
	void NewConnection(void);
	void AddPoll(int fd);
	int readRequest(int fd);
	void DeleteClient(int fd);
	void TimeOutCheck(void);

  private:
	unsigned short ParsePort(char *arg);

	int socket_;
	unsigned short port_;
	std::string password_;
	std::vector<struct pollfd> pollfds_;
	std::vector<Client > clients_;
};

#endif /* IRCD_HPP */
