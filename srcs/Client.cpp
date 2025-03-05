#include "../includes/Client.hpp"

#include <iostream>

Client::Client(int fd, int port, const std::string &hostname)
	: _fd(fd), _port(port), _nickname("*"), _hostname(hostname), _state(HANDSHAKE), _channel(NULL)
{
	(void) _fd;
	(void) _state;
	(void) _channel;
}

void Client::write(const std::string &message) const
{
	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending a message to a client!");
	std::cout << "Send [Server]: " << message << std::endl;
}

std::string Client::getHostname() const
{
	return _hostname;
}

int Client::getPort() const
{
	return _port;
}

std::string Client::getNickname() const
{
	return _nickname;
}

ClientState Client::getClientState() const
{
	return _state;
}

void Client::setClientState(ClientState state)
{
	this->_state = state;
}
