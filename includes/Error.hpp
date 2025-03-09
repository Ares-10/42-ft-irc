#ifndef ERROR_HPP
#define ERROR_HPP

#include <iostream>
#include <string>

class Error {
 public:

  // 451 : 유저 등록이 완전히 되지 않은 경우 (getClientState() != REGISTERED)
  static std::string err_notregistered();

  // 461 : 매개변수가 충분하지 않아 클라이언트 명령을 구문 분석할 수 없는 경우
  static std::string err_needmoreparams(const std::string &nick_name,
                                        const std::string &command);

  // 471 : channel이 (+l) 모드이고 가득 찼을 때
  static std::string err_channelisfull(const std::string &nick_name,
                                       const std::string &channel_name);

  // 403 : 제공된 채널 이름에 대한 채널을 찾을 수 없음을 나타냄.
  static std::string err_nosuchchannel(const std::string &nick_name,
                                       const std::string &channel_name);

  // 405 : JOIN클라이언트가 최대 채널 수에 가입했기 때문에 명령이 실패
  static std::string err_toomanychannels(const std::string &nick_name,
                                         const std::string &channel_name);

  // 475 : JOIN채널에 키가 필요하고 키가 잘못되었거나 제공되지 않았기 때문에
  // 명령이 실패 (+k)
  static std::string err_badchannelkey(const std::string &nick_name,
                                       const std::string &channel_name);

  // 473 : JOIN채널이 [초대 전용] 모드로 설정되어 있고 클라이언트가 채널에 초대
  // 되지 않았거나 초대 예외가 설정되지 않았기 때문에 명령이 실패 (+i)
  static std::string err_inviteonlychan(const std::string &nick_name,
                                        const std::string &channel_name);

  // 476 : JOIN에서 유효하지 않은 channel 형식일 때 발생.
  static std::string err_badchanmask(const std::string &nick_name,
                                     const std::string &command);



    // 525 : channel key가 올바른 format이 아닐 떄.. <- 이거 왜 발동안댐 ㅋㅋ.


// 462
std::string err_unauthorizedcommand(const std::string &client_name,const std::string &command);

// 431
std::string err_nonicknamegiven(const std::string &client_name, const std::string &command);

// 432
std::string err_erroneousnickname(const std::string &client_name,
										const std::string &nickname,
										const std::string &command);

// 433
std::string Error::err_nicknamealreadyuse(const std::string &client_name,
										const std::string &nickname,
										const std::string &command);


	// 451 : 클라이언트가 서버에 등록되지 않음
	static std::string err_notregistered(const std::string &client_name,
										const std::string &command);

	// 997 : 잘못된 비밀번호
	static std::string err_incorrectpassword(const std::string &client_name,
											const std::string &command);

	// 998 : You need to authenticate first
	static std::string err_needtoauth(const std::string &client_name,
									const std::string &command);
};

#endif
