#include "../../includes/Command.hpp"

void Mode::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 2) {  // 461 mode 만 왔을 경우
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_needmoreparams(_client->getNickname(), _command));
  }

  int format_opt = 0;
  Channel *channel_ptr = _server->findChannel(_args[1]);
  if (!Channel::checkChannelNameFormat(_args[1], &format_opt) ||
      channel_ptr == NULL) {  // 채널 양식이 틀렸거나, 채널 존재 x
    if (format_opt == 2)      // 403
      return _client->write(
          ":" + _server->getServerName() + " " +
          Error::err_nosuchchannel(_client->getNickname(), _args[1]));
    else {  // 476 원래는 476이어야 할 것 같은데, libera에서는 479을 뱉음.
            // (공식문서에는 479가 없는 것 같음.)
      return _client->write(":" + _server->getServerName() + " " +
                            Error::err_badchanmask(_args[1]));
    }
  }
  if (_args.size() == 2) {  // mode <channel>
    // 324 : mode 출력
    _client->write(":" + _server->getServerName() + " 324 " +
                   _client->getNickname() + " " +
                   channel_ptr->getChannelName() + " +" +
                   channel_ptr->getChannelMode());
    // 329 : channel 생성 시간 출력
    _client->write(":" + _server->getServerName() + " 329 " +
                   _client->getNickname() + " " +
                   channel_ptr->getChannelName() + " +" +
                   channel_ptr->getChannelGenerateTime());
    return;
  }
  // 혹시나.. mode #yuyu :
  if (_args[2].size() <= 0) return;

  std::string mode_str;
  char prev_sign = 0;  // 이거 생각해보기.
  char curr_sign = '+';
  int arg_idx = 3;

  if (_args[2][0] == '-') {
    curr_sign = '-';
  }

  //   for () 한바퀴 순회하면서 쓰레기 정리 & 에러 체크 461, 472
  // mode #yuyu -i+t-o nick_name 과 같은 경우에서 +t가 나가리가 되면,
  // -io로 뜨는지 -i-o로 뜨는지 체크해보기. => -io로 뜸.
  for (size_t i = 0; i < _args[2].size(); i++) {
    if (_args[2][i] == '+' || _args[2][i] == '-') {
      curr_sign = _args[2][i];
    } else if (Channel::checkChannelModeFormat(_args[2][i])) {
      // if (prev_sign != curr_sign) {
      //   mode_str += curr_sign;
      //   prev_sign = curr_sign;
      // }
      // mode_str += _args[2][i];
      // 위와 똑같이 실행되는지 테스트 하기.
      makeReturnStr(prev_sign, curr_sign, _args[2][i], mode_str);

      // 인자 필요한지 확인하고 필요하다면, 인자 개수가 부족한지 (461인지) 체크
      if ((_args[2][i] == 'k' || _args[2][i] == 'o' ||
           (_args[2][i] == 'l' && curr_sign == '+')) &&
          ++arg_idx > _args.size()) {
        // 인자 개수 부족 (461)
        return _client->write(
            ":" + _server->getServerName() + " " +
            Error::err_needmoreparams(_client->getNickname(), _command));
      }
    } else {
      // 472 : 이상한 channel mode
      return _client->write(
          ":" + _server->getServerName() + " " +
          Error::err_unknownmode(_client->getNickname(), _args[2][i]));
    }
  }
  if (mode_str.length() < 1) return;

  // 다 유효한 입력이라면
  if (channel_ptr->findOperator(_client->getFd()) == NULL) {
    // _client가 channel의 operator가 아니라면
    // 482
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_chanoprivsneeded(_client->getNickname(), _args[1]));
  }

  std::string return_mode_str;
  std::vector<std::string> return_arg_vec;
  arg_idx = 3;
  prev_sign = mode_str[0];
  curr_sign = '+';  // 혹시 모르니까.
  for (size_t i = 0; i < mode_str.length(); i++) {
    if (mode_str[i] == '+' || mode_str[i] == '-') {
      curr_sign = mode_str[i];
      continue;
    }
    if (mode_str[i] == 'i' || mode_str[i] == 't') {  // mode i,t (type d)
      if (curr_sign = '+' && !channel_ptr->findChannelMode(mode_str[i])) {
        // + 인데 없음.
        channel_ptr->addChannelMode(mode_str[i]);
        makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
      } else if (curr_sign = '-' && channel_ptr->findChannelMode(mode_str[i])) {
        // - 인데 있음.
        channel_ptr->removeChannelMode(mode_str[i]);
        makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
      }
    } else if (mode_str[i] == 'o') {
      // mode o 수행 (channel 권한 부여) => user 체크하고.. error handling
      // 401(서버에 있는가) 441(channel에 있는가) 순

      Client *invitee_ptr = _server->findClientByNick(_args[arg_idx]);
      if (invitee_ptr == NULL) {  // server에 초대하고자 하는 client가 없음.
                                  // 401
        _client->write(
            ":" + _server->getServerName() + " " +
            Error::err_nosuchnick(_client->getNickname(), _args[arg_idx]));
        continue;
      } else if (!channel_ptr->findClientByNick(_args[arg_idx])) {
        // 441 channel에 찾고자 하는 client가 없음.
        _client->write(
            ":" + _server->getServerName() + " " +
            Error::err_usernotinchannel(_client->getNickname(), _args[arg_idx],
                                        channel_ptr->getChannelName()));
        continue;
      }
      // 올바르게 존재한다면, operator에 추가
      if (curr_sign = '+')
        channel_ptr->addOperator(invitee_ptr);
      else
        channel_ptr->removeOperator(invitee_ptr->getFd());
      makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
      return_arg_vec.push_back(_args[arg_idx++]);

    } else if (mode_str[i] == 'k') {
      // mode k 수행
      if (curr_sign == '+') {
        // + 면 key 설정.
        std::string key_str;
        for (size_t i = 0; i < _args[arg_idx].length(); i++) {
          if (_args[arg_idx][i] != ':' && _args[arg_idx][i] != ',')
            key_str += _args[arg_idx][i];
        }
        if (key_str.length() > 0) {
          channel_ptr->addChannelMode('k');
          channel_ptr->setChannelKey(key_str);
          makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
          return_arg_vec.push_back(key_str);
        }
      } else if (channel_ptr->findChannelMode('k')) {
        // - 면 channel mode 확인하고 +k일때만 변경.
        channel_ptr->setChannelKey("");
        channel_ptr->removeChannelMode('k');
        makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
        return_arg_vec.push_back("*");
      }
      arg_idx++;
    } else if (mode_str[i] == 'l') {
      // mode l 수행
      if (curr_sign == '+') {
        int limit_number = 0;
        std::stringstream ss;
        std::string check;

        limit_number = atoi(_args[arg_idx].c_str());
        ss << limit_number;
        ss >> check;
        if (limit_number > 0 &&
            (_args[arg_idx] == check ||
             (_args[arg_idx][0] == '+' && _args[arg_idx].substr(1) == check))) {
          // 오버플로우나 이상 없는 양수
          channel_ptr->addChannelMode('l');
          channel_ptr->setClientLimit(limit_number);
          makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
          return_arg_vec.push_back(check);
        }
        arg_idx++;
      } else if (channel_ptr->findChannelMode('l')) {
        // - 옵션이고, channel mode에 l옵션이 있다면,
        channel_ptr->removeChannelMode('l');
        channel_ptr->setClientLimit(std::numeric_limits<unsigned int>::max());
        makeReturnStr(prev_sign, curr_sign, mode_str[i], return_mode_str);
      }

    } else {  // 이미 있거나 굳이 반복 필요 x
      continue;
    }
  }
  // 결과를 각 client에게 출력
  // :nick_name!~user_name@121.135.181.42 MODE #yuyu -o+o nock nock
  if (return_mode_str.length() > 0) {
    std::string write_str = ":" + _client->getClientName() + " MODE " +
                            _args[1] + " " + return_mode_str;
    for (std::vector<std::string>::iterator iter = return_arg_vec.begin();
         iter != return_arg_vec.end(); iter++) {
      write_str += " " + *iter;
    }
    const std::map<int, Client *> client_map = channel_ptr->getClients();
    for (std::map<int, Client *>::const_iterator it = client_map.begin();
         it != client_map.end(); it++) {
      it->second->write(write_str);
    }
  }
}

void Mode::makeReturnStr(char &prev_sign, char curr_sign, char mode_c,
                         std::string &return_mode_str) {
  if (prev_sign != curr_sign) {
    return_mode_str += curr_sign;
    prev_sign = curr_sign;
  }
  return_mode_str += mode_c;
}
