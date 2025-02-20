#include "../includes/Client.hpp"

#include <iostream>

Client::Client(int fd, int port, const std::string &hostname)
	: _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE), _channel(NULL)
{
	(void) _fd;
	(void) _state;
	(void) _channel;
}


std::string Client::getHostname() const
{
	return _hostname;
}

int Client::getPort() const
{
	return _port;
}

void Client::write(const std::string &message) const
{
	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending a message to a client!");
	std::cout << "Send: " << message << std::endl;
}
