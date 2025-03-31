#include "../../includes/Command.hpp"

void Ping::execute()
{
	if (_args.empty())
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), _command));
	_server->sendMessage(_client, "PONG " + _server->getServerName() + " :" + _server->getServerName());
}
