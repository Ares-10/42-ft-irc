#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>

// 이거 걍 안쓸수도..
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
  //   static std::string rpl_invitelist(const std::string &client_name,
  //                                     const std::string &channel_name,
  //                                     const std::string &nick_name,
  //                                     const std::string &setat);

  // 다시 생각해보기 (channel에 접속한 namelist필요)
  // 353 : 명령 에 대한 응답으로 전송되는 NAMES이 숫자는 해당 채널에 가입된
  // 클라이언트 <channel>와 해당 채널의 상태를 나열합니다.
  // symbol부분은 '=' 공개 채널, '@' 는 secret channel인데 우린 +s 옵션이
  // 없으므로 그냥 '=' 로 함. (353이랑 세트)이고, 만약 사용자가 해당 채널이
  // 들어가 있지 않으면, 안보내지는 듯. (366만)
  static std::string rpl_namreply(const std::string &client_name,
                                  const std::string &channel_name,
                                  const std::string &nick_name,
                                  const std::string &setat);

  // 366 : ent as a reply to the NAMES command, this numeric specifies the end
  // of a list of channel member names.
  static std::string rpl_endofname(const std::string &nick_name,
                                   const std::string &channel_name);
};

#endif
