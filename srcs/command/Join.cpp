#include <Error.hpp>

#include "../../includes/Command.hpp"

void Join::execute()
{
	if (_client->getClientState() != REGISTERED)
		throw std::runtime_error(Error::err_notregistered(_client->getNickname(), "JOIN"));
	if (_args.size() < 2)
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), "JOIN"));
	// if (_args.size() == 2)
}
