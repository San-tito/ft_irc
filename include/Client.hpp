#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <netinet/in.h>
# include <string>

class Client
{
  public:
	Client(int fd, struct sockaddr_in addr);
	~Client(void);
	int getFd(void) const;
	int getTime(void) const;

  private:
	int fd_;
	struct sockaddr_in addr_;
	std::string nick_;
	std::string user_;
	time_t log_time_;
	bool is_logged_;
};

#endif /* CLIENT_HPP */
