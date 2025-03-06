#include <Error.hpp>

#include "../../includes/Command.hpp"

void Pass::execute()
{
	if (_args.size() < 1)
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), "PASS"));
	if (_client->getClientState() == REGISTERED)
		throw std::runtime_error(Error::err_unauthorizedcommand(_client->getNickname(), "PASS"));
	if (_args[0] != _server->getPassword())
		throw std::runtime_error(Error::err_incorrectpassword(_client->getNickname(), "PASS"));
	_client->setClientState(LOGIN);
}
