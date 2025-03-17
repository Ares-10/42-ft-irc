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
  if (_args[2].size() == 2) {  // mode <channel>
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

  char prev_sign = '+';
  char cur_sign = '+';
  int arg_idx = 3;
  //   for () 한바퀴 순회하면서 쓰레기 정리 & 에러 체크 461, 472
}
