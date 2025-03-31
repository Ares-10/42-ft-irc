#include "../../includes/Command.hpp"

void User::execute()
{
	if (_client->getClientState() == REGISTERED)
		throw std::runtime_error(Error::err_unauthorizedcommand(_client->getNickname(), _command));
	if (_args.size() < 4)
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), _command));

	_client->setUsername(_args[0]);
	_client->setRealname(_args[3]);
	if (!_client->getNickname().empty())
	{
		_client->setId(_client->getNickname() + "!" + _client->getUsername() + "@" + _client->getHostname());
		if (_client->getClientState() != LOGIN)
			throw std::runtime_error(Error::err_incorrectpassword(_client->getNickname()));
		_client->setClientState(REGISTERED);
		_server->sendMessage(_client, "001", ":Welcome to the Localnet IRC Network " + _client->getId());
	}
}
