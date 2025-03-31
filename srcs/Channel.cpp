#include "Channel.hpp"

Channel::Channel(Server *server, Client *client,
				const std::string &channel_name): _server(server),
												_channel_name(channel_name),
												_channel_generate_time(server->getCurTime())
{
	_channel_topic = "";
	_channel_key = "";
	_channel_mode = "nst";
	_client_limit = std::numeric_limits<unsigned int>::max();

	_clients[client->getFd()] = client;
	_operators[client->getFd()] = client;
}

Channel::Channel(Server *server, const std::string &channel_name)
	: _server(server),
	_channel_name(channel_name),
	_channel_generate_time(server->getCurTime())
{
	_channel_topic = "";
	_channel_key = "";
	_channel_mode = "nst";
	_client_limit = std::numeric_limits<unsigned int>::max();
}

// Getter
const std::string &Channel::getChannelName() const { return _channel_name; }

const std::string &Channel::getChannelTopic() const { return _channel_topic; }

const std::string &Channel::getChannelKey() const { return _channel_key; }

const std::string &Channel::getChannelMode() const { return _channel_mode; }

const std::string &Channel::getChannelTopicSetTime() const
{
	return _channel_topic_set_time;
}

const std::string &Channel::getChannelTopicSetMember() const
{
	return _channel_topic_set_member;
}

const std::map<int, Client *> &Channel::getInvitedClients() const
{
	return _invited_clients;
}

const std::map<int, Client *> &Channel::getClients() const { return _clients; }

const std::map<int, Client *> &Channel::getOperators() const
{
	return _operators;
}

unsigned int Channel::getClientLimit() const { return _client_limit; }

// Setter
void Channel::setChannelTopic(const std::string &channel_topic)
{
	_channel_topic = channel_topic;
}

void Channel::setChannelKey(const std::string &channel_key)
{
	_channel_key = channel_key;
}

void Channel::setChannelMode(const std::string &channel_mode)
{
	_channel_mode = channel_mode;
}

void Channel::setChannelTopicSetTime(
	const std::string &new_channel_topic_set_time)
{
	_channel_topic_set_time = new_channel_topic_set_time;
}

void Channel::setChannelTopicSetMember(
	const std::string &new_channel_topic_set_member)
{
	_channel_topic_set_member = new_channel_topic_set_member;
}

void Channel::setClientLimit(unsigned int client_limit)
{
	_client_limit = client_limit;
}

void Channel::removeClient(int client_fd)
{
	// 클라이언트가 채널에 존재하는지 확인
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	if (it == _clients.end())
		return;

	// 클라이언트가 운영자인 경우 운영자 목록에서도 제거
	removeOperator(client_fd);

	// 클라이언트를 채널에서 제거
	_clients.erase(it);

	// 채널에 남은 클라이언트가 없으면 서버에 채널 삭제
	if (_clients.empty())
		_server->removeChannel(_channel_name);
}

void Channel::removeOperator(int client_fd)
{
	std::map<int, Client *>::iterator it = _operators.find(client_fd);
	if (it != _operators.end())
		_operators.erase(it);
}

void Channel::removeInvitedClient(int client_fd)
{
	std::map<int, Client *>::iterator it = _invited_clients.find(client_fd);
	if (it != _invited_clients.end())
		_invited_clients.erase(it);
}

// 이거 쓴다음 꼭 _client_number 수 확인하기.
bool Channel::addClient(Client *client)
{
	std::map<int, Client *>::iterator it = _clients.find(client->getFd());
	if (it == _clients.end())
	{
		// 없다면
		_clients[client->getFd()] = client;
		return true;
	}
	return false; // 이미 해당하는 사람이 존재함.
}

bool Channel::addOperator(Client *client)
{
	std::map<int, Client *>::iterator it = _operators.find(client->getFd());
	if (it == _operators.end())
	{
		// 없다면
		_operators[client->getFd()] = client;
		return true;
	}
	return false; // 이미 해당하는 사람이 존재함.
}

bool Channel::addInvitedClient(Client *client)
{
	std::map<int, Client *>::iterator it = _invited_clients.find(client->getFd());
	if (it == _invited_clients.end())
	{
		_invited_clients[client->getFd()] = client;
		return true;
	}
	return false; // 이미 해당하는 사람이 존재함.
}

bool Channel::removeChannelMode(char channel_mode)
{
	if (channel_mode == 's' || channel_mode == 'n') return false;
	size_t idx = _channel_mode.find(channel_mode);
	if (idx != std::string::npos)
	{
		// 있다면 지워라
		_channel_mode.erase(idx);
		return true;
	}
	return false;
}

bool Channel::addChannelMode(char channel_mode)
{
	if (!checkChannelModeFormat(channel_mode))
		return false; // i, k, o, l, t만 가능
	size_t idx = _channel_mode.find(channel_mode);
	if (idx == std::string::npos)
	{
		// 없다면 만들어라.
		_channel_mode += channel_mode;
		return true;
	}
	return false;
}

bool Channel::findChannelMode(char channel_mode)
{
	size_t idx = _channel_mode.find(channel_mode);
	if (idx != std::string::npos)
	{
		// 있다면 true
		return true;
	}
	return false;
}

Client *Channel::findInvitedClient(int client_fd)
{
	std::map<int, Client *>::iterator it = _invited_clients.find(client_fd);
	return (it != _invited_clients.end()) ? it->second : NULL;
}

Client *Channel::findClient(int client_fd)
{
	std::map<int, Client *>::iterator it = _clients.find(client_fd);
	return (it != _clients.end()) ? it->second : NULL;
}

Client *Channel::findClientByNick(const std::string &nick_name)
{
	for (std::map<int, Client *>::iterator it = _clients.begin();
		it != _clients.end(); it++)
	{
		if (it->second->getNickname() == nick_name) return it->second;
	}
	return NULL;
}

Client *Channel::findOperator(int client_fd)
{
	std::map<int, Client *>::iterator it = _operators.find(client_fd);
	return (it != _operators.end()) ? it->second : NULL;
}

std::map<std::string, bool> Channel::getClientNamesWithPrefix()
{
	std::map<std::string, bool> return_map;
	for (std::map<int, Client *>::iterator it = _operators.begin();
		it != _operators.end(); it++) // operator
	{
		return_map[it->second->getNickname()] = true;
	}
	for (std::map<int, Client *>::iterator it = _clients.begin();
		it != _clients.end(); it++) // client
	{
		if (return_map.find(it->second->getNickname()) ==
			return_map.end()) // 없다면,
			return_map[it->second->getNickname()] = false;
	}
	return return_map;
}

// 이거 수정해야할듯. :떄문에, 첫 이름에 #오지 않을때와 " "와 같은 문자가
// channel name으로 왔을떄 처리를 다르게 해야함.
bool Channel::checkChannelNameFormat(const std::string &channel_name,
									int *err_code)
{
	for (size_t i = 1; i < channel_name.length(); i++)
	{
		// 479(476)
		if (channel_name[i] == ' ' || channel_name[i] == ',' ||
			!std::isprint(channel_name[i]))
		{
			*err_code = 1;
			return false;
		}
	}
	if (channel_name.length() < 1 || channel_name[0] != '#')
	{
		// 403
		*err_code = 2;
		return false;
	}
	*err_code = 0;
	return true;
}

bool Channel::checkChannelModeFormat(char c)
{
	if (c == 'i' || c == 'k' || c == 'o' || c == 'l' || c == 't') return true;
	return false;
}

// bool Channel::checkChannelKeyFormat(const std::string &channel_key) { // mode
// +k 시
//   if (channel_key.length() < 1 || channel_key)
// }

const std::string &Channel::getChannelGenerateTime() const
{
	return this->_channel_generate_time;
}

void Channel::broadcastMessage(const std::string &message, Client *excludeClient)
{
	for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client = it->second;
		if (excludeClient != NULL && client->getFd() == excludeClient->getFd())
			continue;
		client->write(message);
	}
}