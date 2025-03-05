#include "../../includes/Command.hpp"

void Join::execute()
{
	if (_client->getClientState() != REGISTERED)
	{
		_client->write(":" + _server->getServerName() + " 451 " + _client->getNickname() + " JOIN :You have not registered.");
		return;
	}
	if (_args.size() < 2)
	{
		_client->write("461" + this->_client->getNickname() + ":Not enough parameters");
		return;
	}
	// if (_args.size() == 2)
}
