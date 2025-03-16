#include "../includes/Client.hpp"

#include <iostream>

Client::Client(int fd, int port, const std::string &hostname)
    : _fd(fd), _port(port), _hostname(hostname), _state(HANDSHAKE) {
  (void)_fd;
  (void)_state;
  (void)_channels;
}

void Client::write(const std::string &message) const {
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

std::string Client::getClientName() const {
  return _nickname + "!~" + _username + "@" + _hostname;
}

std::string Client::getId() const { return _id; }

int Client::getFd() const { return _fd; }

ClientState Client::getClientState() const { return _state; }

std::vector<std::string> Client::getChannelNames() const {
  std::vector<std::string> channel_names;
  for (std::map<std::string, Channel *>::const_iterator it = _channels.begin();
       it != _channels.end(); it++) {
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

bool Client::quitChannel(const std::string &channel_name) {
  for (std::map<std::string, Channel *>::iterator it = _channels.begin();
       it != _channels.end(); it++) {
    if (it->second->getChannelName() == channel_name) {
      Channel *temp = it->second;
      _channels.erase(it);          // client에서 해당 channel연결 해제.
      if (temp->removeClient(_fd))  // channel에서 해당 유저 삭제.
      {
        temp->removeOperator(_fd);
        // 삭제 성공
        return true;
      } else {
        std::cout << "quitChannel : " << channel_name
                  << " 삭제 실패 (해당하는 channel이 없음.)\n";
      }
    }
  }
  return false;  // client가 해당 channel에 있지 않음.
}

void Client::quitAllChannel() {
  for (std::map<std::string, Channel *>::iterator it = _channels.begin();
       it != _channels.end(); it++) {
    Channel *temp = it->second;
    _channels.erase(it);          // client에서 해당 channel연결 해제.
    if (temp->removeClient(_fd))  // channel에서 해당 유저 삭제.
    {
      temp->removeOperator(_fd);
      // 삭제 성공
      // 구현 해야함. 아님 그냥 비워두거나..
    } else {
      std::cout << "quitAllChannel : " << temp->getChannelName()
                << " 삭제 실패 (해당하는 channel이 없음.)\n";
    }
  }
}

Channel *Client::findChannel(
    const std::string &channel_name) {  // 없으면 null 반환.

  std::map<std::string, Channel *>::iterator it = _channels.find(channel_name);
  if (it != _channels.end()) {
    return it->second;
  } else
    return NULL;
}

bool Client::addChannel(Channel *channel) {
  std::map<std::string, Channel *>::iterator it =
      _channels.find(channel->getChannelName());
  if (it != _channels.end())  // 이미 있음.
  {
    std::cout << "addChannel : channel " << channel->getChannelName()
              << " already exist!\n";
    return false;
  } else {
    _channels[channel->getChannelName()] = channel;
    return true;
  }
}
