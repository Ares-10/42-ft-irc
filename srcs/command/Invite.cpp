#include "../includes/Command.hpp"

void Invite::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 3) {  // 461
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_needmoreparams(_client->getNickname(), _command));
  }
  // if (_args.size() > 3)  // 암 것도 안함.
  //   return;

  Client *invitee_ptr = _server->findClientByNick(_args[1]);
  if (invitee_ptr == NULL) {  // server에 초대하고자 하는 client가 없음.
    // 401
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_nosuchnick(_client->getNickname(), _args[1]));
  }

  int format_opt = 0;
  Channel *channel_ptr = _server->findChannel(_args[2]);
  if (!Channel::checkChannelNameFormat(_args[2], &format_opt) ||
      channel_ptr == NULL) {  // 채널 양식이 틀렸거나, 채널 존재 x
    if (format_opt == 2)      // 403
      return _client->write(
          ":" + _server->getServerName() + " " +
          Error::err_nosuchchannel(_client->getNickname(), _args[2]));
    // 476 원래는 476이어야 할 것 같은데, libera에서는 479을 뱉음.
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_badchanmask(_args[2]));
  }
  if (channel_ptr->findClient(_client->getFd()) == NULL ||
      _client->findChannel(_args[1]) == NULL) {
    // 442 channel에 client가 없음.
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_notonchannel(_client->getNickname(), _args[2]));
  }
  if (channel_ptr->findClient(invitee_ptr->getFd()) &&
      _client->findChannel(_args[2])) {  // 443 channel에 이미 invitee가 존재.
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_useronchannel(_client->getNickname(), _args[1], _args[2]));
  }
  if (channel_ptr->findOperator(_client->getFd()) ==
      NULL) {  // 482 inviter가 operator가 아님.
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_chanoprivsneeded(_client->getNickname(), _args[2]));
  }
  // invite message보내고, invited_clients에 추가하기.

  channel_ptr->addInvitedClient(invitee_ptr);
  // 341 -> inviter가 회신 받는 내용
  _client->write(":" + _server->getServerName() + " 341 " +
                 _client->getNickname() + " " + _args[1] + " " + _args[2]);
  invitee_ptr->write(_client->getClientName() + " INVITE " +
                     invitee_ptr->getNickname() + " :" + _args[2]);
}