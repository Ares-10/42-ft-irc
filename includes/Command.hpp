#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include "Parser.hpp"
#include "Server.hpp"

class Server;
class Client;

class Command
{
private:
	std::string _prefix;
	std::string _params;

	Client *_client;
	Server *_server;

public:
	void setCommand(Client *client, Server *server, std::string params);

	virtual void execute() = 0;
};

class Notice : public Command
{
public:
	void execute();
};

class PrivMsg : public Command
{
public:
	void execute();
};

class Part : public Command
{
public:
	void execute();
};

class Quit : public Command
{
public:
	void execute();
};

class Join : public Command
{
public:
	void execute();
};

class User : public Command
{
public:
	void execute();
};

class Nick : public Command
{
public:
	void execute();
};

class Pass : public Command
{
public:
	void execute();
};

class Kick : public Command
{
public:
	void execute();
};

class Ping : public Command
{
public:
	void execute();
};

class Pong : public Command
{
public:
	void execute();
};

class Mode : public Command
{
public:
	void execute();
};


#endif //COMMAND_HPP
