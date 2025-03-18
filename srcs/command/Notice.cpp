#include "Command.hpp"

/**
 * NOTICE <nickname> <text>
 */
void Notice::execute()
{
	if (_args.size() < 2 || _args[0].empty() || _args[1].empty())
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), "NOTICE"));

	std::string target = _args[0];
	std::string message = _args[1];

	for (std::vector<std::string>::iterator it = _args.begin() + 2; it != _args.end(); it++)
		message += " " + *it;

	if (message.at(0) == ':')
		message = message.substr(1);


	// to channel
	if (target.at(0) == '#')
	{
		return;
	}

	// to client
	Client *dest = _server->findClient(target);
	if (!dest)
		throw std::runtime_error(Error::err_nosuchnick(_client->getNickname(), "NOTICE"));
	dest->write(":" + _client->getId() + " NOTICE " + target + " :" + message);
}
