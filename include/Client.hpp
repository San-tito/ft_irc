#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
class Client {
	public:
		Client(int fd);
		~Client(void);
		int getFd(void) const;
		int getTime(void) const;
	private:
		int fd_;
		std::string nick_;
		std::string user_;
		time_t log_time_;
		bool is_logged_;
};

#endif /* CLIENT_HPP */
