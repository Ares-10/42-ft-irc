#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Client.hpp"
#include "Server.hpp"

class Server;
class Client;

class Channel {
 private:
  std::string _channel_name;   // channel name
  std::string _channel_topic;  // topic (info)
  std::string _channel_key;    // channel passwd
  std::string _channel_mode;   // +i +t ..?

  // 첫 번쨰 문자는 채널의 접두사여야함 (# : 일반 채널, & : 지역 채널)
  // 제한 사항은 공백 (' ', 0x20), 제어 문자 G / BELL ('^G',
  // 0x07)또는 쉼표 (',', 0x2C)(프로토콜에서 목록 항목 구분
  // 기호로 사용됨)를 포함할 수 없다는 것입니다.
  std::map<int, Client *> _clients;
  Client *_chanops;  // channel operator

  unsigned int _client_limit;
  unsigned int _client_number;
};

#endif  // CHANNEL_HPP
