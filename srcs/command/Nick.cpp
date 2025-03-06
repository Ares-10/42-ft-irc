#include <Error.hpp>

#include "../../includes/Command.hpp"

void Nick::execute()
{
	if (_client->getClientState() != LOGIN)
		throw std::runtime_error(Error::err_needtoauth(_client->getNickname(), "NICK"));
	if (_args.empty())
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), "NICK"));

	int j = 0;
	while (_args[0][j])
	{
		if (!isalnum(_args[0][j]) && _args[0][j] != '-' && _args[0][j] != '_' && _args[0][j] != '\r')
			throw std::runtime_error(Error::err_erroneousnickname(_client->getNickname(), _args[0], "NICK"));
		j++;
	}

	std::vector<std::string> clientNicknames = _server->getClientNicknames();
	if (std::find(clientNicknames.begin(), clientNicknames.end(), _args[0])
		!= clientNicknames.end())
		throw std::runtime_error(Error::err_nicknamealreadyuse(_client->getNickname(), _args[0], "NICK"));

	_client->setNickname(_args[0]);
	_server->addClientNickname(_client->getNickname());
	if (!_client->getUsername().empty())
	{
		_client->setId(_client->getNickname() + "!" + _client->getUsername() + "@" + _client->getHostname());
		_client->setClientState(REGISTERED);
		_server->sendMessage(_client, "001", ":Welcome to the Localnet IRC Network " + _client->getId());
	}
}
