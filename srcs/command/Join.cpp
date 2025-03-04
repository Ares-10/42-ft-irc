#include "../../includes/Command.hpp"

void Join::execute()
{
	if (_params == ":")
	{
		_client->write(":" + _server->getServerName() + " NOTICE * :*** Looking up your hostname...");
		_client->write(":" + _server->getServerName() + " 451 * JOIN :You have not registered.");
	}
}
