#include "../../includes/Command.hpp"

void Topic::execute() {
  if (_client->getClientState() != REGISTERED)
    throw std::runtime_error(Error::err_notregistered(_client->getNickname()));
  if (_args.size() < 1) // 461
    throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), _command));

  int format_opt = 0;
  Channel *channel_ptr = _server->findChannel(_args[0]);
  if (!Channel::checkChannelNameFormat(_args[0], &format_opt) || channel_ptr == NULL) {  // 채널 양식이 틀렸거나, 채널 존재 x
    // 476은 join 만 해당하는 듯?
    // 403
    throw std::runtime_error(Error::err_nosuchchannel(_client->getNickname(), _args[0]));
  }
  if (channel_ptr->findClient(_client->getFd()) == NULL || _client->findChannel(_args[0]) == NULL) {
    // 442 channel에 client 없음.
    throw std::runtime_error(Error::err_notonchannel(_client->getNickname(), _args[0]));
  }
  if (_args.size() == 1) {  // channel의 topic정보 출력
    // channel의 topic 출력 2줄 있어야함.
    if (channel_ptr->getChannelTopic() != "") {
      // 332 RPL_TOPIC
      _client->write(":" + _server->getServerName() + " 332 " +
                     _client->getNickname() + " " + _args[0] + " :" +
                     channel_ptr->getChannelTopic());
      // 333 : <setat> is a unix timestamp.
      _client->write(":" + _server->getServerName() + " 333 " +
                     _client->getNickname() + " " + _args[0] + " " +
                     channel_ptr->getChannelTopicSetMember() + " " +
                     channel_ptr->getChannelTopicSetTime());
    } else {  // 331 : no topic
      _client->write(":" + _server->getServerName() + " 331 " +
                     _client->getNickname() + " " + _args[0] +
                     " :No topic is set");
    }
  } else {  // args.size() > 1
    if (channel_ptr->findChannelMode('t') &&
        channel_ptr->findOperator(_client->getFd()) == NULL) {
      // +t 모드에서 operator가 아니라면
      // 482
      throw std::runtime_error(Error::err_chanoprivsneeded(_client->getNickname(), _args[0]));
    }
    // channel topic 수정.
    channel_ptr->setChannelTopic(_args[1]);
    channel_ptr->setChannelTopicSetMember(_client->getClientName());
    channel_ptr->setChannelTopicSetTime(_server->getCurTime());

    // 변경된 topic 알리기.
    const std::map<int, Client *> client_map = channel_ptr->getClients();
    for (std::map<int, Client *>::const_iterator it = client_map.begin();
         it != client_map.end(); it++) {
      it->second->write(":" + _client->getClientName() + " TOPIC " + _args[0] +
                        " :" + channel_ptr->getChannelTopic());
    }
  }
}
