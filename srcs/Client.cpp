#include "../includes/Client.hpp"

#include <iostream>

Client::Client(int fd, int port, const std::string &hostname)
	: _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE), _channel(NULL)
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

int Client::getPort() const
{
	return _port;
}

int Client::getFd() const
{
	return _fd;
}

std::string Client::getNickname() const
{
	return _nickname;
}

std::string Client::getUsername() const
{
	return _username;
}

std::string Client::getRealname() const
{
	return _realname;
}

std::string Client::getHostname() const
{
	return _hostname;
}

std::string Client::getId() const
{
	return _id;
}

int Client::getFd() const
{
	return _fd;
}

ClientState Client::getClientState() const
{
	return _state;
}

void Client::setClientState(ClientState state)
{
	this->_state = state;
}

void Client::setNickname(const std::string &nickname)
{
	_nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	_username = username;
}

void Client::setRealname(const std::string &realname)
{
	_realname = realname;
}

void Client::setHostname(const std::string &hostname)
{
	_hostname = hostname;
}

void Client::setId(const std::string &id)
{
	_id = id;
}
