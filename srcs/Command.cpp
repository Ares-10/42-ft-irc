#include "../includes/Command.hpp"

Command::~Command() {
}

void Command::setCommand(Client *client, Server *server, const t_command& cmd)
{
	_command = cmd._command;
	_args = cmd._args;
	_client = client;
	_server = server;
}
