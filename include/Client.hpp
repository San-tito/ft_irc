#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include <cstdlib>
# include <ctime>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <unistd.h>

class	Channel;
class	Membership;

class Client
{
  public:
	Client(int fd, short events);
	~Client(void);
	int getFd(void) const;
	bool isRegistered(void) const;
	short getEvents(void) const;
	time_t getLastTime(void) const;
	std::string getNick(void) const;
	std::string getUser(void) const;
	std::string getPassword(void) const;
	std::string getReadBuffer(void) const;
	std::string getWriteBuffer(void) const;
	template <typename T> Client &operator<<(T const &value)
	{
		std::ostringstream newbuffer;
		newbuffer << wbuffer_ << value;
		wbuffer_ = newbuffer.str();
		return (*this);
	}
	void setFd(int fd);
	void setLastTime(time_t last_time);
	void setRegistered(bool registered);
	void setReadBuffer(std::string buffer);
	void setWriteBuffer(std::string buffer);
	void setNick(std::string nick);
	void setUser(std::string user);
	void setPassword(std::string password);
	void setEvents(short events);
	void unsetEvent(short event);
	void unsetReadBuffer(void);
	void unsetWriteBuffer(void);
	static bool IsValidNick(const std::string &nick);
	static Client *Search(const std::string &nick);
	static void Destroy(Client &client);

  private:
	std::string rbuffer_;
	std::string wbuffer_;
	pollfd poll_;
	std::string user_;
	std::string nick_;
	std::string password_;
	time_t last_time_;
	bool registered_;
};

#endif /* CLIENT_HPP */
