#include "../includes/Error.hpp"

// 461
std::string Error::err_needmoreparams(const std::string &nick_name,
									const std::string &command)
{
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
									const std::string &channel_name)
{
	return "403 " + nick_name + " " + channel_name + " :No such channel";
}

// 471 : (+l) 뒷부분부터는 libera 기준
std::string Error::err_channelisfull(const std::string &nick_name,
									const std::string &channel_name)
{
	return "471 " + nick_name + " " + channel_name +
			" :Cannot join channel (+l) - channel is full, try again later";
}

// 473 : (+i) 뒷부분부터는 libera 기준
std::string Error::err_inviteonlychan(const std::string &nick_name,
									const std::string &channel_name)
{
	return "473 " + nick_name + " " + channel_name +
			" :Cannot join channel (+i) - you must be invited";
}

// 475: (+k) 뒷부분부터는 libera 기준
std::string Error::err_badchannelkey(const std::string &nick_name,
									const std::string &channel_name)
{
	return "475 " + nick_name + " " + channel_name +
			" :Cannot join channel (+k) - bad key";
}

// 476 : 유효하지 않은 channel 형식일 때 발생.
std::string Error::err_badchanmask(const std::string &channel_name)
{
	return "476 " + channel_name + " :Bad Channel Mask";
}

// 442 : 클라이언트가 속하지 않은 채널에서 채널에 영향을 미치는 명령을
// 수행하려고 할 때 반환됨.
std::string Error::err_notonchannel(const std::string &nick_name,
									const std::string &channel_name)
{
	return "442 " + nick_name + " " + channel_name +
			" :You're not on that channel";
}

// 482 : 클라이언트에 적절한 채널 권한이 없기 때문에 명령이 실패했음.
std::string Error::err_chanoprivsneeded(const std::string &nick_name,
										const std::string &channel_name)
{
	return "482 " + nick_name + " " + channel_name +
			" :You're not channel operator";
}

// 443 : 클라이언트가 <nick_name>에 해당하는 사용자를 이미 가입한 채널에
// 초대를 시도하면 반환됨.
std::string Error::err_useronchannel(const std::string &inviter_nick_name,
									const std::string &invitee_nick_name,
									const std::string &channel_name)
{
	return "443 " + inviter_nick_name + " " + invitee_nick_name + " " +
			channel_name + " :is already on channel";
}

// 441 : 닉이 채널에 가입되지 않은 경우(예: MODE #channel +o nick)
// 클라이언트가 채널+닉에 영향을 미치는 명령을 수행하려고 할 때 반환
std::string Error::err_usernotinchannel(const std::string &inviter_nick_name,
										const std::string &invitee_nick_name,
										const std::string &channel_name)
{
	return "441 " + inviter_nick_name + " " + invitee_nick_name + " " +
			channel_name + " :They aren't on that channel";
}

// 472 : 클라이언트가 사용하는 모드 문자가 서버에서 인식되지 않음을 나타냅니다
// libera 는 is an unknown ~ 라고 함. 공식은 is unknown
std::string Error::err_unknownmode(const std::string &nick_name,
									char mode_char)
{
	return "472 " + nick_name + " " + mode_char + " :is unknown mode char to me";
}

// 여기부터 아직 테스트 안해봄.
// 404 : PRIVMSG/ NOTICE를 channel에 전달할 수 없음을 나타냄.
std::string Error::err_cannotsendtochan(const std::string &nick_name,
										const std::string &channel_name)
{
	return "404 " + nick_name + " " + channel_name + " :Cannot send to channel";
}

// 411 : PRIVMSG수신자가 지정되지 않아 메시지가 전달되지 않았음을 나타냄.
std::string Error::err_norecipient(const std::string &nick_name,
									const std::string &command)
{
	return "411 " + nick_name + " :No recipient given (" + command + ")";
}

// 412 : PRIVMSG보낼 텍스트가 없어서 메시지가 전달되지 않았음을 나타냄
std::string Error::err_notexttosend(const std::string &nick_name)
{
	return "412 " + nick_name + " :No text to send";
}

std::string Error::err_unauthorizedcommand(const std::string &client_name,
											const std::string &command)
{
	return "462 " + client_name + " " + command +
			" :Unauthorized command (already registered)";
}

std::string Error::err_notregistered(const std::string &client_name)
{
	return "451 " + client_name + " :You have not registered";
}

std::string Error::err_incorrectpassword(const std::string &client_name)
{
	return "464 " + client_name + " :Password incorrect";
}

std::string Error::err_nonicknamegiven(const std::string &client_name)
{
	return "431 " + client_name + " :No nickname given";
}

std::string Error::err_erroneousnickname(const std::string &client_name,
										const std::string &nickname)
{
	return "432 " + client_name + " " + nickname + " :Erroneous nickname";
}

std::string Error::err_nicknamealreadyuse(const std::string &client_name,
										const std::string &nickname)
{
	return "433 " + client_name + " " + nickname + " :Nickname is already in use";
}
