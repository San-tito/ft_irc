/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/16 00:52:14 by sguzman          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include "Cmd.hpp"
# include "Conn.hpp"
# include "Log.hpp"
# include "Membership.hpp"
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

# define TIMEOUT 20
# define LISTEN_ADDR "0.0.0.0"
# define COMMAND_LEN 512
# define READBUFFER_LEN 2048
# define MAX_CHANNEL_LEN 50
# define MAX_NICK_LEN 9
# define MAX_COMMANDS 3

class	Client;
class	Channel;
class	Membership;

class Server
{
  public:
	Server(int argc, char **argv);
	~Server(void);
	void Run(void);
	void Exit(int status);
	int Dispatch(void);
	static int getClient(int fd);
	static void CloseConnection(int fd);
	static std::string password;
	static std::vector<Client> clients;
	static std::vector<Channel> channels;
	static std::vector<Membership> memberships;

  private:
	int sock_;
	unsigned short ParsePort(char *arg);
	void NewConnection(int sock);
	void ReadRequest(int sock);
	void HandleWrite(int sock);
	void TimeOutCheck(void);
	void ProcessBuffers(void);
	void ProcessRequest(Client &client);
};

#endif /* IRCD_HPP */
