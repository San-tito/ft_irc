#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Client.hpp"
# include "Server.hpp"
# include <cstddef>
# include <set>
# include <string>
# include <vector>

class Channel
{
  public:
	Channel(const std::string &name);
	~Channel(void);
	std::string getName(void) const;
	std::set<char> getModes(void) const;
	std::string getTopic(void) const;
	std::string getKey(void) const;
	size_t getMaxUsers(void) const;
	void AddMode(char mode);
	static bool IsValidName(const std::string &name);
	static Channel *Search(const std::string &name);
	static bool Join(Client &client, const std::string &name);
	static void PartAll(Client &client);
	static void Part(Client &client, const std::string &name,
		const std::string &reason);

  private:
	std::string name_;
	std::set<char> modes_;
	std::string topic_;
	std::string key_;
	size_t max_users_;
};

#endif /* CHANNEL_HPP */
