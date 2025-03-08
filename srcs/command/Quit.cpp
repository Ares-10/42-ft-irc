#include "../../includes/Command.hpp"

void Quit::execute()
{
	std::string ret = _client->getId() + "QUIT ";
	if (!_args.empty())
		ret.append(":" + _args[0] + "\n");
	else
		ret.append("\n");
	// 클라이언트가 속한 모든 체널에 메세지 보내기
	// 클라이언트가 속한 모든 체널 떠나기
	// std::map<std::string, Channel *> channels = _client->getJoinedChannels();
	// std::map<std::string, Channel *>::iterator it = channels.begin();
	// while (it != channels.end())
	// {
	// 	sendToAllUsers(it->second, i, ret);
	// 	it++;
	// }
	// this->_clients[i]->leaveAllChannels();
	_server->removeClient(_client);
	close(_client->getFd());
}
