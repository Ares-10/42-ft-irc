#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>

#include "Command.hpp"

class Command;
class Server;
class Client;

class Parser {
public:
	Command *parse(Client *client, Server *server, std::string message);
};



#endif //PARSER_HPP
