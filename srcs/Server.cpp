#include "Server.hpp"

#include "Parser.hpp"

Server::Server(int port, std::string pass)
    : _password(pass), _parser(new Parser()) {
  try {
    _server_name = "ft_irc";
    _server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_server_fd == -1) {
      throw std::runtime_error("Socket creation failed");
    }

    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1) {
      throw std::runtime_error("Binding failed");
    }

    if (listen(_server_fd, 10) == -1) {
      throw std::runtime_error("Listening failed");
    }

    pollfd server_poll = {_server_fd, POLLIN, 0};
    _pfds.push_back(server_poll);
  } catch (...) {
    delete _parser;
    _parser = NULL;  // NULL로 설정하여 소멸자에서 중복 삭제 방지
    throw;
  }
}

void Server::run() {
  std::cout << "Server started..." << std::endl;

  while (true) {
    int ret = poll(_pfds.data(), _pfds.size(), -1);
    if (ret < 0) throw std::runtime_error("poll() failed");

    for (size_t i = 0; i < _pfds.size(); ++i) {
      if (_pfds[i].revents & POLLIN) {
        if (_pfds[i].fd == _server_fd)
          connectClient();
        else
          handleClient(_pfds[i].fd);
      }
    }
  }
}

void Server::handleClient(int fd) {
  char buffer[512];
  int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);

  if (bytes <= 0) {
    // 클라이언트 연결 종료 처리
    close(fd);
    for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end(); ++it) {
      if (it->fd == fd) {
        _pfds.erase(it);
        break;
      }
    }
    removeClient(_clients[fd]);

    // 클라이언트 객체 정리
    _clientBuffers.erase(fd);

    // Client 객체 메모리 해제
    if (_clients.find(fd) != _clients.end()) {
      delete _clients[fd]; // Client 객체 삭제
      _clients.erase(fd);  // 맵에서 제거
    }

    std::cout << "Client disconnected" << std::endl;
  } else {
    buffer[bytes] = '\0';
    std::cout << "Received [Client(" << _clients[fd]->getNickname() << ")]: " << buffer;

    // 이전 버퍼에 새 데이터 추가
    _clientBuffers[fd] += buffer;

    // 개행 문자 위치 찾기
    size_t pos = 0;
    std::string& clientBuffer = _clientBuffers[fd];

    while ((pos = clientBuffer.find('\n')) != std::string::npos) {
      // 완전한 명령어 추출
      std::string line = clientBuffer.substr(0, pos);

      // 버퍼에서 처리된 부분 제거
      clientBuffer.erase(0, pos + 1);

      // 명령어 처리
      if (!line.empty()) {
        try {
          if (line[line.length() - 1] == '\r') line.erase(line.end() - 1);
          Command *cmd = _parser->parse(_clients[fd], this, line);
          if (cmd != NULL) {
            cmd->execute();
            delete cmd; // Command 객체 삭제
          }
        } catch (std::exception &e) {
          this->sendMessage(_clients[fd], e.what());
        }
      }
    }
  }
}

void Server::connectClient() {
  sockaddr_in client_addr;
  socklen_t client_len;
  int client_fd;

  client_len = sizeof(client_addr);
  client_fd = accept(_server_fd, (struct sockaddr *)&client_addr, &client_len);
  if (client_fd == -1)
    std::cout << "accept() error: " << strerror(errno) << std::endl;
  std::cout << "New client connected!" << std::endl;
  pollfd new_client = {client_fd, POLLIN, 0};
  _pfds.push_back(new_client);

  char hostname[NI_MAXHOST];
  int res = getnameinfo((struct sockaddr *)&client_addr, client_len, hostname,
                        NI_MAXHOST, NULL, 0, NI_NUMERICSERV);
  if (res != 0)
    throw std::runtime_error("Error while getting a hostname on a new client!");

  Client *client = new Client(client_fd, ntohs(client_addr.sin_port), hostname);
  _clients.insert(std::make_pair(client_fd, client));

  std::cout << client->getHostname().c_str() << ":" << client->getPort()
            << " has connected." << std::endl;
}

std::string Server::getServerName() const { return _server_name; }

std::string Server::getPassword() const { return _password; }

std::vector<std::string> Server::getClientNicknames() const {
  return _client_nicknames;
}

std::string Server::getCurTime() const {
  std::stringstream ss;
  ss << time(NULL);
  return ss.str();
}

void Server::addClientNickname(const std::string &nickname) {
  _client_nicknames.push_back(nickname);
}

void Server::removeClientNickname(const std::string &nickname) {
  if (nickname.empty()) return;
  _client_nicknames.erase(
      std::find(_client_nicknames.begin(), _client_nicknames.end(), nickname));
}

Server::~Server() {
  for (std::map<int, Client *>::iterator it = _clients.begin();
       it != _clients.end(); it++)
    delete it->second;
  if (_parser) delete _parser;

  // Channel부분 new로 만든다고 가정.
  for (std::map<std::string, Channel *>::iterator it = _channels.begin();
       it != _channels.end(); it++)
    delete it->second;
}

void Server::sendMessage(const Client *client,
                         const std::string &message) const {
  client->write(":" + _server_name + " " + message);
}

void Server::sendMessage(const Client *client, const std::string &num,
                         const std::string &message) const {
  std::string nickname = client->getNickname();
  if (nickname.empty()) nickname = "*";
  sendMessage(client, num + " " + nickname + " " + message + "\n");
}

void Server::removeClient(Client *client) {
  int fd = client->getFd();
  close(fd);
  for (std::vector<pollfd>::iterator it = _pfds.begin(); it != _pfds.end();
       ++it) {
    if (it->fd == fd) {
      _pfds.erase(it);
      break;
    }
  }
  removeClientNickname(client->getNickname());
  _clients.erase(fd);
  std::cout << "Client disconnected (removeClient())" << std::endl;
}

bool Server::addChannel(Channel *channel) {
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

bool Server::removeChannel(
    std::string channel_name)  // findChannel을 이용할 걸 그랬나..
{
  std::map<std::string, Channel *>::iterator it = _channels.find(channel_name);
  if (it != _channels.end())  // 존재함.
  {
    delete it->second;  // channel 삭제
    _channels.erase(it);
    return true;
  } else {
    std::cout << "removeChannel : channel " << channel_name
              << " does not exist!\n";
    return false;
  }
}

Channel *Server::findChannel(const std::string &channel_name) {
  std::map<std::string, Channel *>::iterator it = _channels.find(channel_name);
  if (it != _channels.end()) {
    return it->second;
  } else
    return NULL;
}

Client *Server::findClientByNick(const std::string &nick_name) {
  for (std::map<int, Client *>::iterator it = _clients.begin();
       it != _clients.end(); it++) {
    if (it->second->getNickname() == nick_name) return it->second;
  }
  return NULL;
}
