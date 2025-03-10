#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstdlib>
# include <ctime>
# include <netinet/in.h>
# include <string>
# include <unistd.h>

class Client
{
  public:
	Client(int fd, struct sockaddr_in addr);
	~Client(void);
	int getFd(void) const;
	time_t getLastTime(void) const;
	void setLastTime(time_t last_time);
	bool isRegistered(void) const;

  private:
	int fd_;
	struct sockaddr_in addr_;
	std::string nick_;
	std::string user_;
	time_t last_time_;
	bool registered_;
};

#endif /* CLIENT_HPP */
