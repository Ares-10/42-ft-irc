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

// 476 : 유효하지 않은 channel 형식일 때 발생.
std::string Error::err_badchanmask(const std::string &channel_name) {
  return "476 " + channel_name + " :Bad Channel Mask";
}

// 442 : 클라이언트가 속하지 않은 채널에서 채널에 영향을 미치는 명령을
// 수행하려고 할 때 반환됨.
std::string Error::err_notonchannel(const std::string &nick_name,
                                    const std::string &channel_name) {
  return "442 " + nick_name + " " + channel_name +
         " :You're not on that channel";
}

// 482 : 클라이언트에 적절한 채널 권한이 없기 때문에 명령이 실패했음.
std::string Error::err_chanoprivsneeded(const std::string &nick_name,
                                        const std::string &channel_name) {
  return "482 " + nick_name + " " + channel_name +
         " :You're not channel operator";
}

// 443 : 클라이언트가 <nick_name>에 해당하는 사용자를 이미 가입한 채널에
// 초대를 시도하면 반환됨.
std::string Error::err_useronchannel(const std::string &inviter_nick_name,
                                     const std::string &invitee_nick_name,
                                     const std::string &channel_name) {
  return "443 " + inviter_nick_name + " " + invitee_nick_name + " " +
         channel_name + " :is already on channel";
}

// 401 : 제공된 별명에 대한 클라이언트를 찾을 수 없음을 나타냄.
std::string Error::err_nosuchnick(const std::string &inviter_nick_name,
                                  const std::string &invitee_nick_name) {
  return "401 " + inviter_nick_name + " " + invitee_nick_name +
         " :No such nick/channel";
}

std::string Error::err_unauthorizedcommand(const std::string &client_name,
                                           const std::string &command) {
  return "462 " + client_name + " " + command +
         " :Unauthorized command (already registered)";
}

std::string Error::err_channelisfull(const std::string &client_name,
                                     const std::string &channel_name) {
  return "471 " + client_name + " " + channel_name +
         " :Cannot join channel (+l)";
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
