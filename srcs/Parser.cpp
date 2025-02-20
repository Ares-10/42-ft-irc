#include "Parser.hpp"

Command *Parser::parse(Client *client, Server *server, std::string message)
{
	std::string cmd;
	std::string params;

	size_t spacePos = message.find(' ');
	if (spacePos != std::string::npos) {
		cmd = message.substr(0, spacePos);
		params = message.substr(spacePos + 1);
	} else {
		cmd = message;
		params = "";
	}
	Command *command;
	if (cmd == "NOTICE")
		command = new Notice();
	else if (cmd == "PRIVMSG")
		command = new PrivMsg();
	else if (cmd == "PART")
		command = new Part();
	else if (cmd == "QUIT")
		command = new Quit();
	else if (cmd == "JOIN")
		command = new Join();
	else if (cmd == "USER")
		command = new User();
	else if (cmd == "NICK")
		command = new Nick();
	else if (cmd == "PASS")
		command = new Pass();
	else if (cmd == "KICK")
		command = new Kick();
	else if (cmd == "PING")
		command = new Ping();
	else if (cmd == "PONG")
		command = new Pong();
	else if (cmd == "MODE")
		command = new Mode();
	else if (cmd == "CAP")
	{
		return NULL;
	}
	else
	{
		throw std::runtime_error("===============err cmd===============");
	}
	command->setCommand(client, server, params);
	return command;
}
