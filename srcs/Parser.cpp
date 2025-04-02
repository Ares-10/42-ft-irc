#include "Parser.hpp"

t_command to_command(std::string message);

Command *Parser::parse(Client *client, Server *server, std::string message) {
  t_command cmd;
  Command *command;

  cmd = to_command(message);
  std::cout << "command is : " << cmd._command << std::endl;
  for (std::vector<std::string>::iterator it = cmd._args.begin();
       it != cmd._args.end(); it++) {
    std::cout << "||" << *it << "||";
  }
  std::cout << std::endl;

  if (cmd._invalid_message) {
    std::cout << "command is invalid message\n";
    return NULL;

  } else if (cmd._command == "NOTICE")
    command = new Notice();
  else if (cmd._command == "PRIVMSG")
    command = new PrivMsg();
  else if (cmd._command == "PART")
    command = new Part();
  else if (cmd._command == "QUIT")
    command = new Quit();
  else if (cmd._command == "JOIN")
    command = new Join();
  else if (cmd._command == "USER")
    command = new User();
  else if (cmd._command == "NICK")
    command = new Nick();
  else if (cmd._command == "PASS")
    command = new Pass();
  else if (cmd._command == "KICK")
    command = new Kick();
  else if (cmd._command == "PING")
    command = new Ping();
  else if (cmd._command == "MODE")
    command = new Mode();
  else if (cmd._command == "NAMES")
    command = new Names();
  else if (cmd._command == "INVITE")
    command = new Invite();
  else if (cmd._command == "TOPIC")
    command = new Topic();
  else  // 잘못된 명령어 오류처리 필요
  {
    std::cout << "command is invalid message\n";
    return NULL;
  }
  command->setCommand(client, server, cmd);
  return command;
}

t_command to_command(std::string message) {
  t_command command;
  size_t i = 0;
  size_t j = 0;

  command._invalid_message = false;
  message = Parser::trim(message);
  std::cout << command._invalid_message << std::endl;
  command._invalid_message = 0;
  if (message[i] == ' ' || !message[i]) {
    command._invalid_message = true;
    std::cout << " 1 " << std::endl;
    return (command);
  }
  j = i;
  while (message[i]) {
    if (message[i] == ' ') {
      if (message[i + 1] == ' ') {
        std::cout << " 2 " << std::endl;

        command._invalid_message = true;
        return (command);
      }
      command._args.push_back(message.substr(j, i - j));
      while (message[i] == ' ') i++;
      j = i;
    }
    if (message[i] == ':') {
      if (message[i - 1] != ' ') {
        std::cout << " 3 " << std::endl;

        command._invalid_message = true;
        return (command);
      }
      command._args.push_back(message.substr(i + 1, message.length() - i));
      command._command = command._args[0];
      command._args.erase(command._args.begin());
      return (command);
    }
    i++;
  }

  if (i && message[j]) command._args.push_back(message.substr(j, i - j));
  command._command = command._args[0];
  command._args.erase(command._args.begin());
  return (command);
}

std::string Parser::trim(const std::string &str) {
  std::string::size_type start = 0, end = str.length();
  while (start < end && std::isspace(str[start])) start++;
  while (end > start && std::isspace(str[end - 1])) end--;
  if (start >= end) {
    if (str.length() == 1 && str[0] == ' ') return " ";
    return "";
  }
  return str.substr(start, end - start);
}