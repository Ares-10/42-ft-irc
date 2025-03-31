#include "Command.hpp"

/**
 * NOTICE <nickname> <text>
 */
void Notice::execute()
{
	if (_client->getClientState() != REGISTERED)
		throw std::runtime_error(Error::err_notregistered(_client->getNickname(), _command));
	if (_args.size() < 2)
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), _command));

	std::string target = _args[0];
	std::string message = _args[1];

	// to channel
	if (target.at(0) == '#')
	{
		return;
	}

	// to client
	Client *dest = _server->findClientByNick(target);
	if (!dest)
		throw std::runtime_error(Error::err_nosuchnick(_client->getNickname(), _command));
	dest->write(":" + _client->getId() + " NOTICE " + target + " :" + message);
}
