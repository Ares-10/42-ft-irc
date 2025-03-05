#include "../includes/Command.hpp"

void Command::setCommand(Client *client, Server *server, t_command cmd)
{
	_command = cmd._command;
	_args = cmd._args;
	_client = client;
	_server = server;
}
