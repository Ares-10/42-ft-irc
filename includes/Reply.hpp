#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>

class Reply {
 public:
  // 332 : 채널에 가입했을 때 클라이언트에게 전송되어 채널의 현재 주제<channel>
  // 를 알려줌.
  static std::string rpl_topic(const std::string &client_name,
                               const std::string &channel_name,
                               const std::string &channel_topic);

  // 333 : <nick>클라이언트에게 주제를 설정한 사람( )과 주제를 설정한 시점(
  // <setat>유닉스 타임스탬프)을 알리기 위해 전송됨 . 전송 후 .RPL_TOPIC (332)
  // => 다시 작성하기 번역본 이상해 + setat이 뭔데...
  static std::string rpl_invitelist(const std::string &client_name,
                                    const std::string &channel_name,
                                    const std::string &nick_name,
                                    const std::string &setat);
};

#endif
