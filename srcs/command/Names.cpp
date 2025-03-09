#include "../../includes/Command.hpp"

void Names::execute() // channel_list부분 미완..
{
    size_t channel_count = 0;
    if (_args.size() > 1)
    {
        if (channel_list->clientsFind(_client->getFd())) // 원래 여기서는 필요없지만 NAMES를 위해.
        {
            // 353 :  symbol부분은 '=' 공개 채널, '@' 는 secret channel인데 우린 +s 옵션을 기본으로 생각했으므로 '@' 로 함.
            std::string names_str = ":" + _server->getServerName() + " 353" + _client->getNickname() + " @ " + channel_list->getChannelName() + " :";
            // // user 쭉 넣기.
            std::map<std::string, bool> user_map = channel_list->getClientNamesWithPrefix();
            for (std::map<std::string, bool>::iterator it = user_map.begin(); it != user_map.end(); it++)
            {
                if (it->second == true) // operator
                    names_str += "@";
                names_str += it->first;
                if (std::next(it) != user_map.end())
                    names_str += " ";
            }
            _client->write(names_str);
        }
    }
    // 366
    std::string return_str = ":" + _server->getServerName() + " 366 " + _client->getNickname() + " ";
    if (channel_count == 0)
        return_str += ":End of /NAMES list";
    else if (channel_count == 1) // 미완 channel_list...
    {
        return_str += channel_list->getChannelName() + " :End of /NAMES list";
    }
    else 
        return_str += "* :End of /NAMES list";
    _client->write(return_str);
    return ;
}
