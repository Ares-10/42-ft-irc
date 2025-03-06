#include "Channel.hpp"

Channel::Channel(Client *client, const std::string &channel_name) : _channel_name(channel_name) {
    _channel_topic = "";
    _channel_key = "";
    _channel_mode = "";
    _client_limit = std::numeric_limits<unsigned int>::max();
    _client_number = 1;
    _op_topic_only = false;
    _key_only = false;

    _clients[client->getFd()] = client;
    _operators[client->getFd()] = client;
}

Channel::Channel(const std::string &channel_name) : _channel_name(channel_name) {
     _channel_topic = "";
    _channel_key = "";
    _channel_mode = "";
    _client_limit = std::numeric_limits<unsigned int>::max();
    _client_number = 1;
    _op_topic_only = false;
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

void Channel::setClientNumber(unsigned int client_number) {
  _client_limit = client_number;
}


void Channel::setOpTopicOnly(bool topic_opt) {
  _op_topic_only = topic_opt;
}

void Channel::setKeyOnly(bool key_opt) {
  _key_only = key_opt;
}