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
	Client(void);
	~Client(void);
	int getFd(void) const;
	void setFd(int fd);
	time_t getLastTime(void) const;
	void setLastTime(time_t last_time);
	bool isRegistered(void) const;
	void setRegistered(bool registered);

  private:
	int fd_;
	std::string user_;
	time_t last_time_;
	bool registered_;
};

#endif /* CLIENT_HPP */
