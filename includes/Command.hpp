#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Client.hpp"
#include "Parser.hpp"
#include "Server.hpp"
#include "Error.hpp"
#include "Reply.hpp"

class Server;
class Client;

typedef struct s_command
{
	std::vector<std::string>	_args;
	std::string					_command;
	bool						_invalid_message;
}	t_command;

class Command
{
protected:
	std::vector<std::string>	_args;
	std::string					_command;

	Client *_client;
	Server *_server;

public:
	void setCommand(Client *client, Server *server, t_command cmd);

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
private: 
	std::vector<std::string> _channel;
	std::vector<std::string> _key;

	void makeChannelVec();
	void makeKeyVec();
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
