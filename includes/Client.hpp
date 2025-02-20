#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <Server.hpp>
#include <string>
#include <sys/socket.h>

#include "Channel.hpp"

enum ClientState
{
	HANDSHAKE,
	LOGIN,
	REGISTERED,
	DISCONNECTED
};


class Client {
private:
	int             _fd;
	int             _port;

	std::string     _nickname;
	std::string     _username;
	std::string     _realname;
	std::string     _hostname;

	ClientState     _state;
	Channel*        _channel;

public:

	Client(int fd, int port, const std::string &hostname);

	std::string getHostname() const;
	int getPort() const;

	void write(const std::string& message) const;
};



#endif //CLIENT_HPP
