#include "../includes/Command.hpp"

// Command::Command(Client *client, Server *server, std::string params)
// 	:_params(params), _client(client), _server(server)
// {
// }


void Command::setCommand(Client *client, Server *server, std::string params)
{
	_params = params;
	_client = client;
	_server = server;
}
