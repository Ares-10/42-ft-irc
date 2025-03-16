#include <Command.hpp>

void Part::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 2) {  // 461
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_needmoreparams(_client->getNickname(), _command));
  }
  //   if (_args.size() > 3)  // 암 것도 안함.
  //     return;

  std::string reason_str = " :";
  _args.size() == 3 ? reason_str += _args[2] : reason_str = "";
  size_t pos = 0;
  std::string channel_str;
  int format_opt = 0;  // channel format error code
  for (size_t i = 0; i < _args[1].length(); i++) {
    format_opt = 0;
    if (_args[1][i] == ',') {
      if (pos < i) {
        channel_str = _args[1].substr(pos, i - pos);
        Channel *channel_ptr = _server->findChannel(channel_str);
        if (Channel::checkChannelNameFormat(channel_str, &format_opt) &&
            channel_ptr) {
          if (channel_ptr->findClient(_client->getFd()) &&
              _client->findChannel(channel_str)) {
            // part 명령어 부분 (channel의 모두에게)
            const std::map<int, Client *> client_map =
                channel_ptr->getClients();
            for (std::map<int, Client *>::const_iterator it =
                     client_map.begin();
                 it != client_map.end(); it++) {
              it->second->write(":" + _client->getNickname() + "~!" +
                                _client->getUsername() + "@" +
                                _client->getHostname() + " PART " +
                                channel_str + reason_str);
            }
            // channel 나가기
            _client->quitChannel(channel_str);
            if (channel_ptr->getClientNumber() < 1)  // channel에 아무도 없으면
              _server->removeChannel(channel_str);   // server에서 channel제거

          } else {  // 442 channel에 client 없음.
            _client->write(
                ":" + _server->getServerName() + " " +
                Error::err_notonchannel(_client->getNickname(), channel_str));
          }
        } else {  // 채널 양식이 틀렸거나, 채널 존재 x
          if (format_opt == 1)
            // 476 원래는 476이어야 할 것 같은데, libera에서는 479을 뱉음. //
            // 이거 part도 해당하는지 확인해야 할듯. join만 체크했음.
            _client->write(":" + _server->getServerName() + " " +
                           Error::err_badchanmask(channel_str));
          else {  // 403
            _client->write(
                ":" + _server->getServerName() + " " +
                Error::err_nosuchchannel(_client->getNickname(), channel_str));
          }
        }
      }
      pos = i + 1;
    }
    // 문자열의 끝부분에서 남은 channel처리
    else if (i == _args[1].length() - 1 && pos <= i && _args[1][i] != ',') {
      channel_str = _args[1].substr(pos, i - pos + 1);
      Channel *channel_ptr = _server->findChannel(channel_str);
      if (Channel::checkChannelNameFormat(channel_str, &format_opt) &&
          channel_ptr) {
        if (channel_ptr->findClient(_client->getFd()) &&
            _client->findChannel(channel_str)) {
          // part 명령어 부분 (channel의 모두에게)
          const std::map<int, Client *> client_map = channel_ptr->getClients();
          for (std::map<int, Client *>::const_iterator it = client_map.begin();
               it != client_map.end(); it++) {
            it->second->write(":" + _client->getNickname() + "~!" +
                              _client->getUsername() + "@" +
                              _client->getHostname() + " PART " + channel_str +
                              reason_str);
          }
          // channel 나가기
          _client->quitChannel(channel_str);
          if (channel_ptr->getClientNumber() < 1)  // channel에 아무도 없으면
            _server->removeChannel(channel_str);   // server에서 channel제거

        } else {  // 442 channel에 client 없음.
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_notonchannel(_client->getNickname(), channel_str));
        }
      } else {  // 채널 양식이 틀렸거나, 채널 존재 x
        if (format_opt == 1)
          // 476 원래는 476이어야 할 것 같은데, libera에서는 479을 뱉음.
          _client->write(":" + _server->getServerName() + " " +
                         Error::err_badchanmask(channel_str));
        else {  // 403
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_nosuchchannel(_client->getNickname(), channel_str));
        }
      }
    }
  }
}
