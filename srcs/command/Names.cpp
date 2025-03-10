#include "../../includes/Command.hpp"

void Names::execute()  // channel_list부분 미완..
{
  std::vector<Channel *> channel_vec;
  if (_args.size() > 1) {
    size_t pos = 0;
    std::string args_str;
    for (size_t i = 0; i < _args[1].length(); i++) {
      if (i = 0 && _args[1][i] == ':')  // 맨 앞에 : 이 왔을때에 대한 예외처리
      {
        i++;
        if (1 < _args[1].length() && _args[1][i] == ',') {
          pos = i + 1;
        } else
          pos = i;
      } else if (_args[1][i] == ',') {
        if (pos < i) {
          args_str = _args[1].substr(pos, i - pos);
          Channel *temp_channel = _server->findChannel(args_str);
          if (temp_channel)
            channel_vec.push_back(temp_channel);  // 서버에 존재하는 channel
        }
        pos = i + 1;
      } else if (i == _args[1].length() - 1 && pos <= i && _args[1][i] != ',') {
        args_str = _args[1].substr(pos, i - pos + 1);
        Channel *temp_channel = _server->findChannel(args_str);
        if (temp_channel)
          channel_vec.push_back(temp_channel);  // 서버에 존재하는 channel
      }
    }

    for (std::vector<Channel *>::iterator it = channel_vec.begin();
         it != channel_vec.end(); it++) {
      if ((*it)->clientsFind(_client->getFd()) &&
          _client->findChannel(
              (*it)->getChannelName()))  // channel의 client 목록에 사용자가
                                         // 있고, 사용자가 join 되어있는
                                         // channel중 현재 channel이 있다면
      {
        // 353 :  symbol부분은 '=' 공개 채널, '@' 는 secret channel인데 우린 +s
        // 옵션을 기본으로 생각했으므로 '@' 로 함.
        std::string names_str = ":" + _server->getServerName() + " 353" +
                                _client->getNickname() + " @ " +
                                (*it)->getChannelName() + " :";
        // // user 쭉 넣기.
        std::map<std::string, bool> user_map =
            (*it)->getClientNamesWithPrefix();
        for (std::map<std::string, bool>::iterator it = user_map.begin();
             it != user_map.end(); it++) {
          if (it->second == true)  // operator
            names_str += "@";
          names_str += it->first;
          if (std::next(it) != user_map.end()) names_str += " ";
        }
        _client->write(names_str);
      }
    }
  }
  // 366
  std::string return_str =
      ":" + _server->getServerName() + " 366 " + _client->getNickname() + " ";
  if (channel_vec.size() == 0)
    return_str += ":End of /NAMES list";
  else if (channel_vec.size() == 1) {
    return_str += channel_vec[0]->getChannelName() + " :End of /NAMES list";
  } else
    return_str += "* :End of /NAMES list";
  _client->write(return_str);
  return;
}
