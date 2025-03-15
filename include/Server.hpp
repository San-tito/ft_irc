/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/15 00:57:59 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Cmd.hpp"
# include "Conn.hpp"
# include "Log.hpp"
# include "Parser.hpp"
# include "Sig.hpp"
# include <algorithm>
# include <arpa/inet.h>
# include <cstring>
# include <map>
# include <sstream>
# include <string>
# include <sys/time.h>
# include <vector>

# define LISTEN_ADDR "0.0.0.0"
# define READBUFFER_LEN 2048
# define TIMEOUT 20
# define COMMAND_LEN 512
# define MAX_NICK_LEN 9
# define MAX_COMMANDS 3

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
	void TimeOutCheck(void);
	void ProcessBuffers(void);
	void ProcessRequest(Client &client);
	static int getClient(int fd);
	static void CloseConnection(int fd);
	static std::string password;
	static std::vector<Client> clients;

  private:
	unsigned short ParsePort(char *arg);

	int sock_;
};

#endif /* IRCD_HPP */
