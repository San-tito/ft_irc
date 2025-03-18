/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ncastell <ncastell@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 18:58:41 by sguzman           #+#    #+#             */
/*   Updated: 2025/03/18 21:53:48 by tuta             ###   ########.fr       */
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

#define ERR_NOSUCHNICK(nick, channel)  \
    "401 " + (nick) + " " + (channel) + " :No such nick or channel name"
#define ERR_NICKNAMETOOLONG(nick, target)  \
    "432 " + (nick) + " " + (target) + " :Nickname too long"
#define ERR_ERRONEUSNICKNAME(nick, target) "432 " + (nick)+ " " + (target) +" :Erroneous nickname"
#define ERR_NICKNAMEINUSE(nick, target) "433 " + (nick) + " " + (target) + " :Nickname already in use"
#define ERR_NOTREGISTERED(nick) "451 " + (nick) + " :Connection not registered"
#define ERR_ALREADYREGISTRED(nick) "462 " + (nick) +  " :Connection already registered"
#define ERR_NEEDMOREPARAMS(nick, target) "461 " + ((nick).empty() ? "*" : nick ) + " " + (target) + " :Syntax error"
#define ERR_CHANNELISFULL(nick, target) "471 "+ (nick) + " " + (target) + " :Cannot join channel (+l) -- Channel is full, try later"
#define ERR_INVITEONLYCHAN(nick, target) "473 " + (nick) + " " + (target) + " :Cannot join channel (+i) -- Invited users only"
#define ERR_BADCHANNELKEY(nick, target) "475 " + (nick) + " " + (target) + " :Cannot join channel (+k) -- Wrong channel key"
#define RPL_WELCOME(nick) "001 " + (nick) + " :Welcome to the jungle " + (nick)
#define ERR_NOTONCHANNEL(nick, target) "442 " + (nick) + " " + (target) + " :You are not on that channel"
#define ERR_CHANOPRIVSNEEDED(nick, target) "482 " + (nick) + " " + (target) + " :You are not channel operator"
#define ERR_NOSUCHCHANNEL(nick, target) "403 " + (nick) + " " + (target) + " :No such channel"
#define ERR_NORECIPIENT(nick, target) "411 " + (nick) + " :No recipient given " + (target)
#define ERR_NOTEXTTOSEND(nick) "412 " + (nick) + " :No text to send"
#define RPL_NOTOPIC(nick, target) "331 " + (nick) + " " + (target) + " :No topic is set"
#define ERR_USERSDONTMATCH(nick) "502 " + (nick) + " :Can't set/get mode for other users"
#define ERR_NOCHANMODES(nick, target) "477 " + (nick) + " " + (target) + " :Channel doesn't support modes"
#define ERR_INVALIDMODEPARAM(nick, target, param) "696 " + (nick) + " " + (target) + " " + (param) + " * :Invalid mode parameter"
#define ERR_USERNOTINCHANNEL(nick, target, param) "441 " + (nick) + " " + (target) + " " + (param) + " :They aren't on that channel"
#define ERR_CHANOPPRIVTOOLOW(nick, target) "482 " + (nick) + " " + (target) + " :Your privileges are too low"

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
	static std::string password;
	static std::vector<Client *> clients;
	static std::vector<Channel *> channels;
	static std::vector<Membership *> memberships;

  private:
	int sock_;
	unsigned short ParsePort(char *arg);
	void NewConnection(int sock);
	void ReadRequest(Client *client);
	void HandleWrite(Client *client);
	void ProcessRequest(Client *client);
	void ProcessBuffers(void);
	void TimeOutCheck(void);
};

#endif /* IRCD_HPP */
