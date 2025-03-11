#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <cstdlib>
#include <ctime>
#include <netinet/in.h>
#include <poll.h>
#include <string>
#include <unistd.h>

class Client {
public:
  Client(int fd, short events);
  ~Client(void);
  int getFd(void) const;
  void setFd(int fd);
  short getEvents(void) const;
  void setEvents(short events);
  time_t getLastTime(void) const;
  void setLastTime(time_t last_time);
  bool isRegistered(void) const;
  void setRegistered(bool registered);
  void setReadBuffer(std::string buffer);
  std::string getReadBuffer(void) const;

private:
  std::string buffer_;
  pollfd poll_;
  std::string user_;
  time_t last_time_;
  bool registered_;
};

#endif /* CLIENT_HPP */
