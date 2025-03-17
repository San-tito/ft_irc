#ifndef MEMBERSHIP_HPP
# define MEMBERSHIP_HPP

# include "Channel.hpp"
# include "Client.hpp"
# include <cstddef>
# include <set>
# include <string>

class	Channel;

class Membership
{
  public:
	Membership(Client *client, Channel *channel);
	~Membership(void);
	Client *getClient(void);
	Channel *getChannel(void);
	void AddMode(char mode);
	static Membership *Get(Client *client, Channel *channel);

  private:
	Client *client_;
	Channel *channel_;
	std::set<char> modes_;
};

#endif /* MEMBERSHIP_HPP */
