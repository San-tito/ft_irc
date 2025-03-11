/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sguzman <sguzman@student.42barcelona.com>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/11 16:11:25 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Conn.hpp"
# include "Log.hpp"
# include "Sig.hpp"
# include <algorithm>
# include <arpa/inet.h>
# include <map>
# include <sstream>
# include <string>
# include <sys/time.h>
# include <vector>

# define LISTEN_ADDR "0.0.0.0"
# define READBUFFER_LEN 2048
# define TIMEOUT 20

class Server
{
  public:
	Server(int argc, char **argv);
	~Server(void);
	void Run(void);
	void Exit(int status);
	void NewConnection(int sock);
	int Dispatch(void);
	void ReadRequest(int sock);
	void HandleWrite(int sock);
	void CloseConnection(int fd);
	void TimeOutCheck(void);
	int getClient(int fd);
	void ProcessBuffers(void);

  private:
	unsigned short ParsePort(char *arg);

	int sock_;
	unsigned short port_;
	std::string password_;
	std::vector<Client> clients_;
};

#endif /* IRCD_HPP */
