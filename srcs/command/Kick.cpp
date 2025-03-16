#include "../../includes/Command.hpp"

void Kick::execute() {
  if (_client->getClientState() != REGISTERED) {  // 451
    return _client->write(":" + _server->getServerName() + " " +
                          Error::err_notregistered());
  }
  if (_args.size() < 3) {  // 461
    return _client->write(
        ":" + _server->getServerName() + " " +
        Error::err_needmoreparams(_client->getNickname(), _command));
  }
  //   if (_args.size() > 3)  // 암 것도 안함.
  //     return;
  //
}
