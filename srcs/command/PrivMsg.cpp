#include "../../includes/Command.hpp"

void PrivMsg::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 2) {
    // 411 arg 1개
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_norecipient(_client->getNickname(), "PRIVMSG"));
  }
  if (_args.size() == 2 || _args[2].length() == 0) {
    // 412 PRIVMSG보낼 텍스트가 없음.
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notexttosend(_client->getNickname()));
  }

  // client, channel로 쪼개기
  // int 0 = client, 1 = channel, 2 = channel (only operator)
  makeClientChannelVec();

  // 해당 쌍 계산하기.
  for (std::vector<std::pair<int, std::string> >::iterator it =
           _nick_channel_vec.begin();
       it != _nick_channel_vec.end(); it++) {
    if (it->first == 0) {  // 0 client
      _server->findClientByNick(it->second)
          ->write(":" + _client->getClientName() + " PRIVMSG " + it->second +
                  " :" + _args[2]);
    } else if (it->first == 1) {  // 1 channel
      std::map<int, Client *> client_map =
          _server->findChannel(it->second)->getClients();
      for (std::map<int, Client *>::const_iterator const_it =
               client_map.begin();
           const_it != client_map.end(); const_it++) {
        if (_client->getFd() == const_it->second->getFd())  // 같은 사람
          continue;
        const_it->second->write(":" + _client->getClientName() + " PRIVMSG " +
                                it->second + " :" + _args[2]);
      }
    } else {  // 2 channel operator
      std::map<int, Client *> operator_map =
          _server->findChannel(it->second)->getOperators();
      for (std::map<int, Client *>::const_iterator const_it =
               operator_map.begin();
           const_it != operator_map.end(); const_it++) {
        if (_client->getFd() != const_it->second->getFd()) continue;
        const_it->second->write(":" + _client->getClientName() + " PRIVMSG @" +
                                it->second + " :" + _args[2]);
      }
    }
  }
}

void PrivMsg::makeClientChannelVec() {
  size_t pos = 0;
  std::string temp_str;
  for (size_t i = 0; i < _args[1].length(); i++) {
    if (_args[1][i] == ',') {
      if (pos < i) {
        temp_str = _args[1].substr(pos, i - pos);
        if (_server->findClientByNick(temp_str)) {
          // server에 찾고자 하는 client 존재
          _nick_channel_vec.push_back(std::make_pair(0, temp_str));
        } else if (temp_str.length() > 2 && temp_str[0] == '@' &&
                   _server->findChannel(temp_str.substr(1))) {
          // server에 channel이 존재하고, operator 모드
          if (_server->findChannel(temp_str.substr(1))
                  ->findOperator(_client->getFd()) == NULL)  // 482
            _client->write(
                ":" + _server->getServerName() + " " +
                Error::err_chanoprivsneeded(_client->getNickname(), temp_str));
          else
            _nick_channel_vec.push_back(std::make_pair(2, temp_str.substr(1)));

        } else if (_server->findChannel(temp_str)) {
          // server에 channel이 존재.
          if (_server->findChannel(temp_str)->findClient(_client->getFd()) ==
              NULL)  // 404 channel 소속이 아니여서 privmsg 전달 불가.
            _client->write(
                ":" + _server->getServerName() + " " +
                Error::err_cannotsendtochan(_client->getNickname(), temp_str));
          else
            _nick_channel_vec.push_back(std::make_pair(1, temp_str));
        } else {
          // 401 server에 찾고자 하는 client가 없음. + channel도 없음.
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_nosuchnick(_client->getNickname(), temp_str));
        }
      }
      pos = i + 1;
    } else if (i == _args[1].length() - 1 && pos <= i) {
      temp_str = _args[1].substr(pos, i - pos + 1);
      if (_server->findClientByNick(temp_str)) {
        // server에 찾고자 하는 client 존재
        _nick_channel_vec.push_back(std::make_pair(0, temp_str));
      } else if (temp_str.length() > 2 && temp_str[0] == '@' &&
                 _server->findChannel(temp_str.substr(1))) {
        // server에 channel이 존재하고, operator 모드
        if (_server->findChannel(temp_str.substr(1))
                ->findOperator(_client->getFd()) == NULL)  // 482
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_chanoprivsneeded(_client->getNickname(), temp_str));
        else
          _nick_channel_vec.push_back(std::make_pair(2, temp_str.substr(1)));

      } else if (_server->findChannel(temp_str)) {
        // server에 channel이 존재.
        if (_server->findChannel(temp_str)->findClient(_client->getFd()) ==
            NULL)  // 404 channel 소속이 아니여서 privmsg 전달 불가.
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_cannotsendtochan(_client->getNickname(), temp_str));
        else
          _nick_channel_vec.push_back(std::make_pair(1, temp_str));
      } else {
        // 401 server에 찾고자 하는 client가 없음. + channel도 없음.
        _client->write(":" + _server->getServerName() + " " +
                       Error::err_nosuchnick(_client->getNickname(), temp_str));
      }
    }
  }
}
