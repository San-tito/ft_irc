#ifndef MESSAGES_HPP
# define MESSAGES_HPP

# define RPL_WELCOME(nick, user) "001 " + (nick) + " :Welcome to the Internet Relay Network " + (nick) + "!" + (user)
# define RPL_YOURHOST(servername, version) "002 :Your host is " + (servername) + ", running version " + (version)
# define RPL_CREATED(date) "003 :This server was created " + (date)
# define RPL_MYINFO(servername, version, user_modes, channel_modes) "004 " + (servername) + " " + (version) + " " + (user_modes) + " " + (channel_modes)

# define ERR_NOSUCHNICK(nick, channel)  "401 " + (nick) + " " + (channel) + " :No such nick or channel name"
# define ERR_NOSUCHCHANNEL(nick, target) "403 " + (nick) + " " + (target) + " :No such channel"
# define ERR_NORECIPIENT(nick, target) "411 " + (nick) + " :No recipient given " + (target)
# define ERR_NOTEXTTOSEND(nick) "412 " + (nick) + " :No text to send"
# define ERR_NICKNAMETOOLONG(nick, target)  "432 " + (nick) + " " + (target) + " :Nickname too long"
# define ERR_ERRONEUSNICKNAME(nick, target) "432 " + (nick)+ " " + (target) +" :Erroneous nickname"
# define ERR_NICKNAMEINUSE(nick, target) "433 " + (nick) + " " + (target) + " :Nickname already in use"
# define ERR_NOTONCHANNEL(nick, target) "442 " + (nick) + " " + (target) + " :You are not on that channel"
# define ERR_USERNOTINCHANNEL(nick, target, param) "441 " + (nick) + " " + (target) + " " + (param) + " :They aren't on that channel"
# define ERR_NEEDMOREPARAMS(nick, target) "461 " + ((nick).empty() ? "*" : nick ) + " " + (target) + " :Syntax error"
# define ERR_ALREADYREGISTRED(nick) "462 " + (nick) +  " :Connection already registered"
# define ERR_CHANNELISFULL(nick, target) "471 "+ (nick) + " " + (target) + " :Cannot join channel (+l) -- Channel is full, try later"
# define ERR_INVITEONLYCHAN(nick, target) "473 " + (nick) + " " + (target) + " :Cannot join channel (+i) -- Invited users only"
# define ERR_BADCHANNELKEY(nick, target) "475 " + (nick) + " " + (target) + " :Cannot join channel (+k) -- Wrong channel key"
# define ERR_NOCHANMODES(nick, target) "477 " + (nick) + " " + (target) + " :Channel doesn't support modes"
# define ERR_INVALIDMODEPARAM(nick, target, param) "696 " + (nick) + " " + (target) + " " + (param) + " * :Invalid mode parameter"
# define ERR_CHANOPPRIVTOOLOW(nick, target) "482 " + (nick) + " " + (target) + " :Your privileges are too low"
# define ERR_CHANOPRIVSNEEDED(nick, target) "482 " + (nick) + " " + (target) + " :You are not channel operator"
# define ERR_USERSDONTMATCH(nick) "502 " + (nick) + " :Can't set/get mode for other users"

#endif /* MESSAGES_HPP */
