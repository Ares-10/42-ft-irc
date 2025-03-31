#include <Error.hpp>

#include "../../includes/Command.hpp"

void Pass::execute()
{
	if (_args.empty())
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), _command));
	if (_client->getClientState() == REGISTERED)
		throw std::runtime_error(Error::err_unauthorizedcommand(_client->getNickname(), _command));
	if (_args[0] != _server->getPassword())
		throw std::runtime_error(Error::err_incorrectpassword(_client->getNickname(), _command));
	_client->setClientState(LOGIN);
}
