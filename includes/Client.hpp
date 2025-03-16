#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <sys/socket.h>

#include <Server.hpp>
#include <map>
#include <string>
#include <vector>

#include "Channel.hpp"

class Channel;

enum ClientState { HANDSHAKE, LOGIN, REGISTERED, DISCONNECTED };

class Client {
 private:
  int _fd;
  int _port;

  // static int _max_channel;  // 가입 가능한 채널의 최대 값. 이거 서버 설정인데
  // 서버가 가지고 있어야 하나?

  std::string _nickname;  // 이거 유일해야함.
  std::string _username;  // user 이름
  std::string _realname;  // 실제 이름 ??
  std::string _hostname;  // client ip 주소

  std::string _id;

  ClientState _state;
  std::map<std::string, Channel *> _channels;  // key : channel_name

 public:
  Client(int fd, int port, const std::string &hostname);

  int getFd() const;
  int getPort() const;
  std::string getNickname() const;
  std::string getUsername() const;
  std::string getRealname() const;
  std::string getHostname() const;
  std::string getClientName() const;
  std::string getId() const;
  ClientState getClientState() const;
  std::vector<std::string> getChannelNames() const;

  void setNickname(const std::string &nickname);
  void setUsername(const std::string &username);
  void setRealname(const std::string &realname);
  void setHostname(const std::string &hostname);
  void setId(const std::string &id);
  void setClientState(ClientState state);

  // 나간 channel의 이름을 return
  bool quitChannel(const std::string &channel_name);
  void quitAllChannel();

  Channel *findChannel(const std::string &channel_name);  // 없으면 null 반환.
  bool addChannel(Channel *channel);

  void write(const std::string &message) const;
};

#endif  // CLIENT_HPP
