#include "Channel.hpp"

Channel::Channel(Server *server, Client *client,
                 const std::string &channel_name)
    : _server(server), _channel_name(channel_name) {
  _channel_topic = "";
  _channel_key = "";
  _channel_mode = "nst";
  _client_limit = std::numeric_limits<unsigned int>::max();
  _client_number = 1;

  _clients[client->getFd()] = client;
  _operators[client->getFd()] = client;
}

Channel::Channel(Server *server, const std::string &channel_name)
    : _server(server), _channel_name(channel_name) {
  _channel_topic = "";
  _channel_key = "";
  _channel_mode = "nst";
  _client_limit = std::numeric_limits<unsigned int>::max();
  _client_number = 0;
}

// Getter
const std::string &Channel::getChannelName() const { return _channel_name; }

const std::string &Channel::getChannelTopic() const { return _channel_topic; }

const std::string &Channel::getChannelKey() const { return _channel_key; }

const std::string &Channel::getChannelMode() const { return _channel_mode; }

const std::map<int, Client *> &Channel::getInvitedClients() const {
  return _invited_clients;
}

const std::map<int, Client *> &Channel::getClients() const { return _clients; }

const std::map<int, Client *> &Channel::getOperators() const {
  return _operators;
}

unsigned int Channel::getClientLimit() const { return _client_limit; }

unsigned int Channel::getClientNumber() const { return _client_number; }

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

bool Channel::increaseClientNumber() {
  if (_client_number + 1 >= _client_limit) return false;
  _client_number += 1;
  return true;
}

bool Channel::decreaseClientNumber() {
  _client_number -= 1;
  if (_client_number <= 0) return false;
  return true;
}

bool Channel::removeClient(
    int client_fd) {  // 이거 쓴다음 꼭 _client_number 수 확인하기.
  std::map<int, Client *>::iterator it = _clients.find(client_fd);
  if (it != _clients.end()) {
    _clients.erase(it);
    decreaseClientNumber();
    return true;
  }
  return false;  // 못찾음. (삭제 실패)
}

bool Channel::removeOperator(int client_fd) {
  std::map<int, Client *>::iterator it = _operators.find(client_fd);
  if (it != _operators.end()) {
    _operators.erase(it);
    return true;
  }
  return false;  // 못찾음. (삭제 실패)
}

bool Channel::removeInvitedClient(int client_fd) {
  std::map<int, Client *>::iterator it = _invited_clients.find(client_fd);
  if (it != _invited_clients.end()) {
    _invited_clients.erase(it);
    return true;
  }
  return false;  // 못찾음. (삭제 실패)
}

void Channel::removeChannelMode(const std::string &channel_mode) {
  for (size_t i = 0; i < channel_mode.length(); i++) {
    if (channel_mode[i] == 's' || channel_mode[i] == 'n') continue;
    size_t idx = _channel_mode.find(channel_mode[i]);
    if (idx != std::string::npos) {
      // 있다면 지워라
      _channel_mode.erase(idx);
    }
  }
}

void Channel::addChannelMode(const std::string &channel_mode) {
  for (size_t i = 0; i < channel_mode.length(); i++) {
    size_t idx = _channel_mode.find(channel_mode[i]);
    if (idx == std::string::npos) {
      // 없다면 만들어라.
      _channel_mode += channel_mode[i];
    }
  }
}

bool Channel::findChannelMode(const std::string &channel_mode) {
  for (size_t i = 0; i < channel_mode.length(); i++) {
    size_t idx = _channel_mode.find(channel_mode[i]);
    if (idx != std::string::npos) {
      return true;
    }
  }
  return false;
}

Client *Channel::invited_clientsFind(int client_fd) {
  std::map<int, Client *>::iterator it = _invited_clients.find(client_fd);
  return (it != _invited_clients.end()) ? it->second : NULL;
}

Client *Channel::clientsFind(int client_fd) {
  std::map<int, Client *>::iterator it = _clients.find(client_fd);
  return (it != _clients.end()) ? it->second : NULL;
}

Client *Channel::operatorsFind(int client_fd) {
  std::map<int, Client *>::iterator it = _operators.find(client_fd);
  return (it != _operators.end()) ? it->second : NULL;
}

std::map<std::string, bool> Channel::getClientNamesWithPrefix() {
  std::map<std::string, bool> return_map;
  for (std::map<int, Client *>::iterator it = _operators.begin();
       it != _operators.end(); it++)  // operator
  {
    return_map[it->second->getNickname()] = true;
  }
  for (std::map<int, Client *>::iterator it = _clients.begin();
       it != _clients.end(); it++)  // client
  {
    if (return_map.find(it->second->getNickname()) ==
        return_map.end())  // 없다면,
      return_map[it->second->getNickname()] = false;
  }
  return return_map;
}

bool Channel::checkChannelNameFormat(const std::string &channel_name) {
  if (channel_name.length() < 1 || channel_name[0] != '#') return false;
  for (size_t i = 1; i < channel_name.length(); i++) {
    if (channel_name[i] == ' ' || !std::isprint(channel_name[i])) return false;
  }
  return true;
}

// bool Channel::checkChannelKeyFormat(const std::string &channel_key) { // mode
// +k 시
//   if (channel_key.length() < 1 || channel_key)
// }
