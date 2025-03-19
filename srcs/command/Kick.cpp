#include "../../includes/Command.hpp"

void Kick::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 3) {  // 461
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_needmoreparams(_client->getNickname(), _command));
  }
  //   if (_args.size() > 3)  // 암 것도 안함.
  //     return;

  int format_opt = 0;
  size_t idx = 0;
  Channel *channel_ptr;
  for (idx = 0; idx < _args[1].length(); idx++) {
    if (_args[1][idx] == ',') {  // libera 기준 channel 부분이 , 까지만 해석함.
      channel_ptr = _server->findChannel(_args[1].substr(0, idx));
      break;
    }
  }
  if (idx == _args[1].length()) channel_ptr = _server->findChannel(_args[1]);
  if (!Channel::checkChannelNameFormat(_args[1], &format_opt) ||
      channel_ptr == NULL) {  // 403 채널 양식이 틀렸거나, 채널 존재 x
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_nosuchchannel(_client->getNickname(), _args[1]));
  }
  if (channel_ptr->findClient(_client->getFd()) == NULL ||
      _client->findChannel(_args[1]) == NULL) {
    // 442 channel에 client가 없음.
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_notonchannel(_client->getNickname(), _args[1]));
  }
  if (channel_ptr->findOperator(_client->getFd()) == NULL) {
    // 482 You are not a channel operator
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_chanoprivsneeded(_client->getNickname(), _args[1]));
  }
  makeClientVec(channel_ptr);  // _kickClientVec 만들기

  const std::map<int, Client *> client_map = channel_ptr->getClients();
  for (std::vector<std::string>::iterator it = _kickClientVec.begin();
       it != _kickClientVec.end(); it++) {  // kick해야할 user에 대해
    for (std::map<int, Client *>::const_iterator iter = client_map.begin();
         iter != client_map.end(); iter++) {  // channel의 모두에게 message 전송
      std::string write_str = _client->getClientName() + " KICK " +
                              channel_ptr->getChannelName() + " " + *it + " :";
      _args.size() < 4 ? write_str += *it : write_str += _args[3];
      iter->second->write(write_str);
    }
    // client(kick 당한 사람)에서 channel 삭제 => channel에서 해당 client 삭제
    channel_ptr->findClientByNick(*it)->quitChannel(
        channel_ptr->getChannelName());
    // // channel에서 해당 client 삭제 .. quitChannel에서 알아서 처리해줌.
    // channel_ptr->removeOperator(channel_ptr->findClientByNick(*it)->getFd());
    // channel_ptr->removeClient(channel_ptr->findClientByNick(*it)->getFd());
  }

  // channel 삭제 여부 확인
  if (channel_ptr->getClientNumber() < 1)  // channel에 아무도 없으면
    _server->removeChannel(
        channel_ptr->getChannelName());  // server에서 channel제거
}

void Kick::makeClientVec(Channel *channel_ptr) {
  size_t pos = 0;
  std::string client_str;
  for (size_t i = 0; i < _args[2].length(); i++) {
    if (_args[2][i] == ',') {
      if (pos < i) {
        client_str = _args[2].substr(pos, i - pos);
        if (_server->findClientByNick(client_str)) {
          // server에 찾고자 하는 client 존재
          if (channel_ptr->findClientByNick(client_str)) {
            // 해당 channel에도 찾고자 하는 client 존재
            _kickClientVec.push_back(client_str);
          } else {
            // 441 channel에 찾고고자 하는 client가 없음.
            _client->write(
                ":" + _server->getServerName() + " " +
                Error::err_usernotinchannel(_client->getNickname(), client_str,
                                            channel_ptr->getChannelName()));
          }
        } else {
          // 401 server에 찾고자 하는 client가 없음.
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_nosuchnick(_client->getNickname(), client_str));
        }
      }
      pos = i + 1;
    } else if (i == _args[2].length() - 1 && pos <= i) {
      client_str = _args[2].substr(pos, i - pos + 1);
      if (_server->findClientByNick(client_str)) {
        // server에 찾고자 하는 client 존재
        if (channel_ptr->findClientByNick(client_str)) {
          // 해당 channel에도 찾고자 하는 client 존재
          _kickClientVec.push_back(client_str);
        } else {
          // 441 channel에 찾고자 하는 client가 없음.
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_usernotinchannel(_client->getNickname(), client_str,
                                          channel_ptr->getChannelName()));
        }
      } else {
        // 401 server에 찾고자 하는 client가 없음.
        _client->write(
            ":" + _server->getServerName() + " " +
            Error::err_nosuchnick(_client->getNickname(), client_str));
      }
    }
  }
}
