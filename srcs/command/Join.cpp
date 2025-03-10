#include "../../includes/Command.hpp"

void Join::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 2) {  // 461
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_needmoreparams(_client->getNickname(), _command));
  }
  if (_args.size() > 3)  // 암 것도 안함.
    return;

  if (_args.size() == 2 && _args[1] == "0") {  // channel 다 나감. (part 이용)
    std::vector<std::string> channel_names = _client->getChannelNames();
    std::string arguments;
    t_command cmd;
    cmd._invalid_message = false;
    cmd._command = "PART";
    cmd._args.push_back("PART");
    for (std::vector<std::string>::iterator it = channel_names.begin();
         it != channel_names.end(); it++) {
      arguments += *it;
    }
    if (arguments.length() > 0) {
      cmd._args.push_back(arguments);
      Command *command = new Part();
      command->setCommand(_client, _server, cmd);
      command->execute();
      delete command;
    }
    return;
  }
  // channel부분 잘라내기.
  this->makeChannelVec();
  if (_args.size() == 3)  // key 존재
    this->makeKeyVec();

  // join 수행.
  for (size_t i = 0; i < _channels.size(); i++) {
    Channel *channel_ptr = _server->findChannel(_channels[i]);
    if (!channel_ptr) {
      // 해당 channel이 server에 없음. => create channel // 미완
      Channel *new_channel = new Channel(_server, _client, _channels[i]);
      _server->addChannel(new_channel);
      _client->addChannel(new_channel);

      // key 설정 부분
      // if (_keys.size() > i) // 이 부분 libera랑 inspircd(irssi에서 시도
      // 시 알아서 제거하여 전송)에서는 필요없는 듯 => 없애도 될듯..
      // {
      // 	new_channel->setKeyOnly(true);
      // 	new_channel->setChannelKey(_keys[i]);
      // }

      // join 명령어 부분 (channel의 모두에게)
      // tcp 113 포트로 ident인증 x => '~' 추가
      _client->write(":" + _client->getNickname() + "~!" +
                     _client->getUsername() + "@" + _client->getHostname() +
                     " JOIN " + _channels[i]);

      // :molybdenum.libera.chat MODE #yuyu +Cnst 이거 모드는 초기 생성 시에만
      // 나오는 듯?
      _client->write(":" + _server->getServerName() + " MODE " + _channels[i] +
                     " +" + new_channel->getChannelMode());

      // topic(없으므로 생략) 해야홤.

      // names 부분
      t_command cmd;
      cmd._invalid_message = false;
      cmd._command = "NAMES";
      cmd._args.push_back("NAMES");
      cmd._args.push_back(_channels[i]);
      Command *command = new Names();
      command->setCommand(_client, _server, cmd);
      command->execute();
      delete command;
    } else {
      // 해당 channel이 이미 존재.
      if () {  //
        //
      }
    }
  }
}

void Join::makeChannelVec() {
  size_t pos = 0;
  std::string channel_str;
  for (size_t i = 0; i < _args[1].length(); i++) {
    if (_args[1][i] == ',') {
      if (pos < i) {
        channel_str = _args[1].substr(pos, i - pos);
        if (Channel::checkChannelNameFormat(channel_str))
          _channels.push_back(channel_str);
        else {  // 403 원래는 476이어야 할 것 같은데, libera에서는 403을 뱉음.
          _client->write(
              ":" + _server->getServerName() + " " +
              Error::err_nosuchchannel(_client->getNickname(), channel_str));
        }
      }
      pos = i + 1;
    } else if (i == _args[1].length() - 1 && pos <= i && _args[1][i] != ',') {
      channel_str = _args[1].substr(pos, i - pos + 1);
      if (Channel::checkChannelNameFormat(channel_str))
        _channels.push_back(channel_str);
      else {  // 403
        _client->write(
            ":" + _server->getServerName() + " " +
            Error::err_nosuchchannel(_client->getNickname(), channel_str));
      }
    }
  }
}

void Join::makeKeyVec() {
  size_t pos = 0;
  std::string key_str;
  for (size_t i = 0; i < _args[2].length(); i++) {
    if (i = 0 && _args[2][i] == ':')  // 맨 앞에 : 이 왔을때에 대한 예외처리
    {
      i++;
      if (1 < _args[2].length() && _args[2][i] == ',') {
        pos = i + 1;
      } else
        pos = i;
    } else if (_args[2][i] == ',') {
      if (pos < i) {
        key_str = _args[2].substr(pos, i - pos);
        _keys.push_back(key_str);
      }
      pos = i + 1;
    } else if (i == _args[2].length() - 1 && pos <= i && _args[1][i] != ',') {
      key_str = _args[2].substr(pos, i - pos + 1);
      _keys.push_back(key_str);
    }
  }
}
