#include "Error.hpp"

// 451
std::string Error::err_notregistered(){
  return "451 * :You have not registered"; // * 대신 client name 이 들어가야하나 그런 경우가 없음. ㅋㅋ
}

// 461
std::string Error::err_needmoreparams(const std::string &nick_name,
                                        const std::string &command) {
                                 return "461 " + nick_name + " " + command + " :Not enough parameters";         
                                        }

// 403 
std::string Error::err_nosuchchannel(const std::string &nick_name,
                                       const std::string &channel_name){
return "403 " + nick_name + " " + channel_name + " :No such channel";
                                       }



std::string Error::err_channelisfull(const std::string &nick_name,
                                     const std::string &channel_name) {
  return nick_name + " " + channel_name + " :Cannot join channel (+l)";
}
