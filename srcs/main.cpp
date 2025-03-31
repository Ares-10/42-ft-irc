#include "../includes/Server.hpp"

int main(int argc, char **argv) {
  if (argc != 3) {
    std::cerr << "Usage : ./ircserv <port> <password>" << std::endl;
    return 1;
  }

  try {
    Server server(atoi(argv[1]), argv[2]);
    server.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  return 0;
}
