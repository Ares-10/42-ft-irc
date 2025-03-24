#include "Error.hpp"

// 451
std::string Error::err_notregistered() {
  return "451 * :You have not registered";  // * 대신 client name 이
                                            // 들어가야하나 그런 경우가 없음.
                                            // ㅋㅋ
}

// 461
std::string Error::err_needmoreparams(const std::string &nick_name,
                                      const std::string &command) {
  return "461 " + nick_name + " " + command + " :Not enough parameters";
}

// 401
std::string Error::err_nosuchnick(const std::string &sender_nickname,
								const std::string &target_nickname)
{
	return "401 " + sender_nickname + " " + target_nickname + " :No such nick/channel";
}


// 403
std::string Error::err_nosuchchannel(const std::string &nick_name,
                                     const std::string &channel_name) {
  return "403 " + nick_name + " " + channel_name + " :No such channel";
}

// 471 : (+l) 뒷부분부터는 libera 기준
std::string Error::err_channelisfull(const std::string &nick_name,
                                     const std::string &channel_name) {
  return "471 " + nick_name + " " + channel_name +
         " :Cannot join channel (+l) - channel is full, try again later";
}

// 473 : (+i) 뒷부분부터는 libera 기준
std::string Error::err_inviteonlychan(const std::string &nick_name,
                                      const std::string &channel_name) {
  return "473 " + nick_name + " " + channel_name +
         " :Cannot join channel (+i) - you must be invited";
}

// 475: (+k) 뒷부분부터는 libera 기준
std::string Error::err_badchannelkey(const std::string &nick_name,
                                     const std::string &channel_name) {
  return "475 " + nick_name + " " + channel_name +
         " :Cannot join channel (+k) - bad key";
}

std::string Error::err_unauthorizedcommand(const std::string &client_name,
                                           const std::string &command) {
  return "462 " + client_name + " " + command +
         " :Unauthorized command (already registered)";
}

std::string Error::err_notregistered(const std::string &client_name,
                                     const std::string &command) {
  return "451 " + client_name + " " + command + " :You have not registered.";
}

std::string Error::err_incorrectpassword(const std::string &client_name,
                                         const std::string &command) {
  return "997 " + client_name + " " + command + " :Incorrect password.";
}

std::string Error::err_needtoauth(const std::string &client_name,
                                  const std::string &command) {
  return "998 " + client_name + " " + command +
         " :You need to authenticate first.";
}

std::string Error::err_nonicknamegiven(const std::string &client_name,
                                       const std::string &command) {
  return "431 " + client_name + " " + command + " :No nickname given.";
}

std::string Error::err_erroneousnickname(const std::string &client_name,
                                         const std::string &nickname,
                                         const std::string &command) {
  return "432 " + client_name + " " + command + " " + nickname +
         " :Erroneous nickname.";
}

std::string Error::err_nicknamealreadyuse(const std::string &client_name,
                                          const std::string &nickname,
                                          const std::string &command) {
  return "433 " + client_name + " " + command + " " + nickname +
         " :Nickname is already in use.";
}
