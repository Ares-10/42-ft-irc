#include "../../includes/Command.hpp"

void Pass::execute()
{
	if (_args.size() < 1)
	{
		_client->write("461" + _client->getNickname() + "PASS :Not enough parameters");
		return;
	}
	if (_client->getClientState() == REGISTERED)
	{
		_client->write("462" + _client->getNickname() + " :Unauthorized command (already registered)");
		return;
	}
	if (_args[0] != _server->getPassword())
	{
		_client->write("997" + _client->getNickname() + " :Incorrect password");
		return;
	}
	else
		_client->setClientState(REGISTERED);
}
