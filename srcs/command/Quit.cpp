#include "../../includes/Command.hpp"

void Quit::execute()
{
	std::string quitMessage;
	if (!_args.empty())
		quitMessage = _args[0];
	if (_client->getClientState() == REGISTERED)
	{
		std::string notification = ":" + _client->getId() + " QUIT";
		if (!quitMessage.empty())
			notification += " :" + quitMessage;
		// 참여 중인 모든 채널에 QUIT 메시지 브로드캐스트
		_client->broadcastToAllChannels(notification);
	}
	_client->quitAllChannel();
	_server->removeClient(_client);
}
