#include "Channel.hpp"

Channel::Channel(Server *server, Client *client, const std::string &channel_name) : _server(server), _channel_name(channel_name) {
    _channel_topic = "";
    _channel_key = "";
    _channel_mode = "";
    _client_limit = std::numeric_limits<unsigned int>::max();
    _client_number = 1;
    _op_topic_only = true;
    _key_only = false;

    _clients[client->getFd()] = client;
    _operators[client->getFd()] = client;
}

Channel::Channel(Server *server, const std::string &channel_name) : _server(server) ,_channel_name(channel_name) {
     _channel_topic = "";
    _channel_key = "";
    _channel_mode = "";
    _client_limit = std::numeric_limits<unsigned int>::max();
    _client_number = 0;
    _op_topic_only = true;
    _key_only = false;
}


// Getter
const std::string &Channel::getChannelName() const {
  return _channel_name;
}

const std::string &Channel::getChannelTopic() const {
  return _channel_topic;
}

const std::string &Channel::getChannelKey() const {
  return _channel_key;
}

const std::string &Channel::getChannelMode() const {
  return _channel_mode;
}

const std::map<int, Client *> &Channel::getInvitedClients() const {
  return _invited_clients;
}

const std::map<int, Client *> &Channel::getClients() const {
  return _clients;
}

const std::map<int, Client *> &Channel::getOperators() const {
  return _operators;
}

unsigned int Channel::getClientLimit() const {
  return _client_limit;
}

unsigned int Channel::getClientNumber() const {
  return _client_number;
}

bool Channel::isOpTopicOnly() const {
  return _op_topic_only;
}

bool Channel::isKeyOnly() const {
  return _key_only;
}



// Setter
void Channel::setChannelTopic(const std::string &channel_topic) {
  _channel_topic = channel_topic;
}

void Channel::setChannelKey(const std::string &channel_key) {
  _channel_key = channel_key;
}

void Channel::setChannelMode(const std::string &channel_mode) {
  _channel_mode = channel_mode;
}

void Channel::setClientLimit(unsigned int client_limit) {
  _client_limit = client_limit;
}

// void Channel::setClientNumber(unsigned int client_number) {
//   _client_limit = client_number;
// }


void Channel::setOpTopicOnly(bool topic_opt) {
  _op_topic_only = topic_opt;
}

void Channel::setKeyOnly(bool key_opt) {
  _key_only = key_opt;
}

bool Channel::increaseClientNumber() {
  if (_client_number + 1 >= _client_limit)
    return false;
  _client_number += 1;
  return true;
}

bool Channel::decreaseClientNumber() {
  _client_number -= 1;
  if (_client_number <= 0)
    return false;
  return true;
}


bool Channel::removeClient(int fd) { // 이거 쓴다음 꼭 _client_number 수 확인하기.
  std::map<int, Client *>::iterator it = _clients.find(fd);
  if (it != _clients.end())
  {
    _clients.erase(it);
    decreaseClientNumber();
    return true;
  }
  return false; // 못찾음. (삭제 실패)
} 

bool Channel::removeOperator(int fd) {
  std::map<int, Client *>::iterator it = _operators.find(fd);
  if (it != _operators.end())
  {
    _operators.erase(it);
    return true;
  }
  return false; // 못찾음. (삭제 실패)
} 

bool Channel::removeInvitedClient(int fd) {
  std::map<int, Client *>::iterator it = _invited_clients.find(fd);
  if (it != _invited_clients.end())
  {
    _invited_clients.erase(it);
    return true;
  }
  return false; // 못찾음. (삭제 실패)
} 




bool Channel::checkChannelNameFormat(const std::string &channel_name){
  if (channel_name.length() < 1 || channel_name[0] != '#')
    return false;
  for (size_t i = 1; i < channel_name.length(); i++)
  {
    if (channel_name[i] == ' ' || !std::isprint(channel_name[i]))
      return false;
  }
  return true;
}

// bool Channel::checkChannelKeyFormat(const std::string &channel_key) { // mode +k 시
//   if (channel_key.length() < 1 || channel_key)
// }


