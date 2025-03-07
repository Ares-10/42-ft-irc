#ifndef IRCSERVER_HPP
# define IRCSERVER_HPP

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/poll.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream>

#include "Client.hpp"
#include "Parser.hpp"

class Client;
class Parser;

class Server
{
private:
	int _server_fd;
	std::string _password;
	std::vector<pollfd> _pfds;
	std::map<int, Client *> _clients;
	std::map<int, Channel *> _channels;

	std::string _server_name;

	Parser *_parser;

	void handleClient(int fd);

	void connectClient();

public:
	Server(int port, std::string pass);
	~Server();

	void run();

	std::string getServerName() const;
	std::string getPassword() const;
};

#endif
