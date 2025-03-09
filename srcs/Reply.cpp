#include "Reply.hpp"


// 353 : /NAMES에 대한 응답. iridium.libera.chat 353 nickname @ #y :nickname @MAGIC
// std::string Reply::rpl_namreply(const std::string &client_name,
//                                     const std::string &channel_name,
//                                     const std::string &nick_name,
//                                     const std::string &setat) {
//                                         return "353 " + nick_name + " = " + channel_name + " :";
//                                     }


// 366 : /NAMES의 응답의 마지막 부분 
std::string Reply::rpl_endofname(const std::string &nick_name, const std::string &channel_name) {
    return "366 " + nick_name + " " + channel_name + " :End of /NAMES list";
}
