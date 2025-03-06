#include "Reply.hpp"


// 366 
static std::string rpl_endofname(const std::string &client_name, const std::string &channel_name) {
    return client_name + " " + channel_name + " :End of /NAMES list";
}
