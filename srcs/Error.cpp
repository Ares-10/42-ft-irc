#include "Error.hpp"

std::string Error::err_needmoreparams(std::string &client_name,
                                      std::string &command) {
  return client_name + " " + command + " :Not enough parameters";
}

std::string Error::err_channelisfull(std::string &client_name,
                                     std::string &channel_name) {
  return client_name + " " + channel_name + " :Cannot join channel (+l)";
}
