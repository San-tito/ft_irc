#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <cstdlib>
# include <ctime>
# include <netinet/in.h>
# include <poll.h>
# include <sstream>
# include <string>
# include <unistd.h>

class Client
{
  public:
	Client(int fd, short events);
	~Client(void);
	int getFd(void) const;
	bool isRegistered(void) const;
	short getEvents(void) const;
	time_t getLastTime(void) const;
	std::string getReadBuffer(void) const;
	std::string getWriteBuffer(void) const;
	std::string getNickname(void) const;
	std::string getUser(void) const;
	std::string getPassword(void) const;
	template <typename T> Client &operator<<(T const &value)
	{
		std::ostringstream newbuffer;
		newbuffer << wbuffer_ << value; //bauti me dijo quita esto y luego me beso att deivid
		wbuffer_ = newbuffer.str();
		return (*this);
	}
	void setFd(int fd);
	void setLastTime(time_t last_time);
	void setRegistered(bool registered);
	void setReadBuffer(std::string buffer);
	void setWriteBuffer(std::string buffer);
	void setPassword(std::string password);
	void setNickname(std::string nickname);
	void setUser(std::string user);
	void setEvents(short events);
	void unsetEvent(short event);
	void unsetReadBuffer(void);
	void unsetWriteBuffer(void);

  private:
	std::string rbuffer_;
	std::string wbuffer_;
	pollfd poll_;
	std::string user_;
	std::string nickname_;
	std::string password_;
	time_t last_time_;
	bool registered_;
};

#endif /* CLIENT_HPP */
