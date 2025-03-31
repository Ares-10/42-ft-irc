#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include <string>

class Error {
 public:
  // 401 : 제공된 별명에 대한 클라이언트를 찾을 수 없음을 나타냄.
  static std::string err_nosuchnick(const std::string &inviter_nick_name,
                                    const std::string &invitee_nick_name);

  // 403 : 제공된 채널 이름에 대한 채널을 찾을 수 없음을 나타냄.
  static std::string err_nosuchchannel(const std::string &nick_name,
                                       const std::string &channel_name);

  // 404 : PRIVMSG/ NOTICE를 channel에 전달할 수 없음 을 나타냄.
  // 채널이 조정 되어 클라이언트가 채널에서 말할 수 있는 권한이 없거나, 외부
  // 메시지 모드가 설정되지 않은 채널에 참여하지 않은 경우
  static std::string err_cannotsendtochan(const std::string &nick_name,
                                          const std::string &channel_name);

  // 405 : JOIN클라이언트가 최대 채널 수에 가입했기 때문에 명령이 실패
  static std::string err_toomanychannels(const std::string &nick_name,
                                         const std::string &channel_name);

  // 411 : PRIVMSG수신자가 지정되지 않아 메시지가 전달되지 않았음을 나타냄.
  static std::string err_norecipient(const std::string &nick_name,
                                     const std::string &command);

  // 412 : PRIVMSG보낼 텍스트가 없어서 메시지가 전달되지 않았음을 나타냄,
  static std::string err_notexttosend(const std::string &nick_name);

  // 441 : 닉이 채널에 가입되지 않은 경우(예: MODE #channel +o nick)
  // 클라이언트가 채널+닉에 영향을 미치는 명령을 수행하려고 할 때 반환
  static std::string err_usernotinchannel(const std::string &inviter_nick_name,
                                          const std::string &invitee_nick_name,
                                          const std::string &channel_name);

  // 442 : 클라이언트가 속하지 않은 채널에서 채널에 영향을 미치는 명령을
  // 수행하려고 할 때 반환됨.
  static std::string err_notonchannel(const std::string &nick_name,
                                      const std::string &channel_name);

  // 443 : 클라이언트가 <nick_name>에 해당하는 사용자를 이미 가입한 채널에
  // 초대를 시도하면 반환됨.
  static std::string err_useronchannel(const std::string &inviter_nick_name,
                                       const std::string &invitee_nick_name,
                                       const std::string &channel_name);

  // 451 : 유저 등록이 완전히 되지 않은 경우 (getClientState() != REGISTERED)
  static std::string err_notregistered();

  // 461 : 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없는 경우
  static std::string err_needmoreparams(const std::string &nick_name,
                                        const std::string &command);

	// 471 : channel이 (+l) 모드이고 가득 찼을 때
	static std::string err_channelisfull(const std::string &nick_name,
										const std::string &channel_name);

  // 472 : 클라이언트가 사용하는 모드 문자가 서버에서 인식되지 않음을 나타냅니다
  static std::string err_unknownmode(const std::string &nick_name,
                                     char mode_char);

  // 475 : JOIN채널에 키가 필요하고 키가 잘못되었거나 제공되지 않았기 때문에
  // 명령이 실패 (+k)
  static std::string err_badchannelkey(const std::string &nick_name,
                                       const std::string &channel_name);

  // 476 : 유효하지 않은 channel 형식일 때 발생.
  // 이거 libera에서는 479로 나오는 듯.
  static std::string err_badchanmask(const std::string &channel_name);
  
	// 473 : JOIN채널이 [초대 전용] 모드로 설정되어 있고 클라이언트가 채널에 초대
	// 되지 않았거나 초대 예외가 설정되지 않았기 때문에 명령이 실패 (+i)
	static std::string err_inviteonlychan(const std::string &nick_name,
										const std::string &channel_name);


  // 482 : 클라이언트에 적절한 채널 권한이 없기 때문에 명령이 실패했음.
  static std::string err_chanoprivsneeded(const std::string &nick_name,
                                          const std::string &channel_name);

  // 525 : channel key가 올바른 format이 아닐 떄.. <- 이거 왜 발동안댐 ㅋㅋ.

  // 462
  static std::string err_unauthorizedcommand(const std::string &client_name,
                                             const std::string &command);

  // 431
  static std::string err_nonicknamegiven(const std::string &client_name,
                                         const std::string &command);

  // 432
  static std::string err_erroneousnickname(const std::string &client_name,
                                           const std::string &nickname,
                                           const std::string &command);

  // 433
  static std::string err_nicknamealreadyuse(const std::string &client_name,
                                            const std::string &nickname,
                                            const std::string &command);

  // 451 : 클라이언트가 서버에 등록되지 않음
  static std::string err_notregistered(const std::string &client_name,
                                       const std::string &command);

  // 464 : 잘못된 비밀번호
  static std::string err_incorrectpassword(const std::string &client_name,
                                           const std::string &command);
};

#endif
