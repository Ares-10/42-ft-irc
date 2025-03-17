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
  char prev_sign = '+';
  int arg_idx = 3;

  if (_args[2][0] == '-') {
    prev_sign = '-';
    mode_str += '-';
  } else
    mode_str += '+';

  //   for () 한바퀴 순회하면서 쓰레기 정리 & 에러 체크 461, 472
  // 여기 미완. mode #yuyu -i+t-o nick_name 과 같은 경우에서 +t가 나가리가 되면,
  // -io로 뜨는지 -i-o로 뜨는지 체크해보기.
  for (size_t i = 0; i < _args[2].size(); i++) {
    if (_args[2][i] == '+' || _args[2][i] == '-') {
      if (prev_sign != _args[2][i] && i < _args[2].size() - 1 &&
          Channel::checkChannelModeFormat(_args[2][i + 1])) {
        mode_str += _args[2][i];
      }
    } else if (Channel::checkChannelModeFormat(_args[2][i])) {
      // 인자 확인하고 461인지 아닌지 체크
    } else {
      // 472 : 이상한 channel mode
      return _client->write(
          ":" + _server->getServerName() + " " +
          Error::err_unknownmode(_client->getNickname(), _args[2][i]));
    }
  }
}
