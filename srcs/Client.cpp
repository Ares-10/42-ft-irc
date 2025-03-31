#include "../includes/Client.hpp"

#include <iostream>

Client::Client(int fd, int port, const std::string &hostname)
	: _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE)
{
	(void) _fd;
	(void) _state;
	(void) _channels;
}

void Client::write(const std::string &message) const
{
	std::string buffer = message + "\r\n";
	if (send(_fd, buffer.c_str(), buffer.length(), 0) < 0)
		throw std::runtime_error("Error while sending a message to a client!");
	std::cout << "Send [Server]: " << message << std::endl;
}

int Client::getPort() const { return _port; }

int Client::getFd() const { return _fd; }

std::string Client::getNickname() const { return _nickname; }

std::string Client::getUsername() const { return _username; }

std::string Client::getRealname() const { return _realname; }

std::string Client::getHostname() const { return _hostname; }

std::string Client::getClientName() const
{
	return _nickname + "!~" + _username + "@" + _hostname;
}

std::string Client::getId() const { return _id; }

ClientState Client::getClientState() const { return _state; }

std::vector<std::string> Client::getChannelNames() const
{
	std::vector<std::string> channel_names;
	for (std::map<std::string, Channel *>::const_iterator it = _channels.begin();
		it != _channels.end(); it++)
	{
		channel_names.push_back(it->second->getChannelName());
	}
	return channel_names;
}

void Client::setClientState(ClientState state) { this->_state = state; }

void Client::setNickname(const std::string &nickname) { _nickname = nickname; }

void Client::setUsername(const std::string &username) { _username = username; }

void Client::setRealname(const std::string &realname) { _realname = realname; }

void Client::setHostname(const std::string &hostname) { _hostname = hostname; }

void Client::setId(const std::string &id) { _id = id; }

void Client::quitChannel(const std::string &channel_name)
{
	std::map<std::string, Channel *>::iterator it = _channels.find(channel_name);

	// 채널을 찾음
	if (it != _channels.end())
	{
		Channel *channel = it->second;
		_channels.erase(it); // client에서 해당 channel 연결 해제
		channel->removeClient(_fd);
	}
}

void Client::broadcastToAllChannels(const std::string &message)
{
	// 현재 가입한 모든 채널에 메시지 브로드캐스트
	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
	{
		Channel *channel = it->second;

		// 채널의 모든 클라이언트 가져오기
		std::map<int, Client *> channelClients = channel->getClients(); // 이 메서드가 있다고 가정

		// 채널의 각 클라이언트에게 메시지 전송 (자신 제외)
		for (std::map<int, Client *>::iterator clientIt = channelClients.begin();
			clientIt != channelClients.end(); ++clientIt)
		{
			if (clientIt->first != this->_fd)
				clientIt->second->write(message);
		}
	}
}

void Client::quitAllChannel()
{
	std::vector<Channel *> channelsToQuit;

	for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); ++it)
		channelsToQuit.push_back(it->second);

	// 채널 맵 비우기
	_channels.clear();

	// 각 채널에서 클라이언트 제거
	for (std::vector<Channel *>::iterator it = channelsToQuit.begin(); it != channelsToQuit.end(); ++it)
	{
		Channel *channel = *it;
		channel->removeClient(_fd);
	}
}

Channel *Client::findChannel(const std::string &channel_name)
{
	// 없으면 null 반환.
	std::map<std::string, Channel *>::iterator it = _channels.find(channel_name);
	if (it != _channels.end())
		return it->second;
	return NULL;
}

bool Client::addChannel(Channel *channel)
{
	std::map<std::string, Channel *>::iterator it =
			_channels.find(channel->getChannelName());
	if (it != _channels.end()) // 이미 있음.
	{
		std::cout << "addChannel : channel " << channel->getChannelName()
				<< " already exist!\n";
		return false;
	} else
	{
		_channels[channel->getChannelName()] = channel;
		return true;
	}
}
