#ifndef IRCSERVER_HPP
#define IRCSERVER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "Channel.hpp"
#include "Client.hpp"
#include "Parser.hpp"

class Client;
class Parser;
class Channel;

class Server {
 private:
  int _server_fd;
  std::string _password;
  std::vector<pollfd> _pfds;
  std::map<int, Client *> _clients;
  std::map<std::string, Channel *> _channels;  // key : channel_name

  std::vector<std::string> _client_nicknames;

  std::string _server_name;

  Parser *_parser;

  void handleClient(int fd);

  void connectClient();

 public:
  Server(int port, std::string pass);
  ~Server();

  void run();

  std::string getServerName() const;
  std::string getPassword() const;
  std::vector<std::string> getClientNicknames() const;
  std::string getCurTime() const;

  void addClientNickname(const std::string &nickname);
  void removeClientNickname(const std::string &nickname);
  void removeClient(Client *client);

  bool addChannel(Channel *channel);
  bool removeChannel(
      std::string channel_name);  // &는 혹시 문제가 될 수 있으므로.. (del 후에)
  Channel *findChannel(const std::string &channel_name);  // 없으면 null 반환.
  Client *findClientByNick(const std::string &nick_name);

  void sendMessage(const Client *client, const std::string &message) const;
  void sendMessage(const Client *client, const std::string &num,
                   const std::string &message) const;
};

#endif
