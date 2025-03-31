#include "Command.hpp"

/**
 * NOTICE <nickname> <text>
 */
void Notice::execute()
{
	if (_client->getClientState() != REGISTERED)
		throw std::runtime_error(Error::err_notregistered(_client->getNickname(), _command));
	if (_args.size() < 2)
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), _command));

	std::string targets = _args[0];
	std::string message = _args[1];

	// 쉼표로 구분된 대상들을 분리
	std::vector<std::string> targetList;
	size_t pos = 0;
	std::string token;
	std::string delimiterComma = ",";

	// 쉼표로 구분된 모든 대상을 벡터에 추가
	while ((pos = targets.find(delimiterComma)) != std::string::npos) {
		token = targets.substr(0, pos);
		if (!token.empty()) {
			targetList.push_back(token);
		}
		targets.erase(0, pos + delimiterComma.length());
	}
	// 마지막 대상 추가
	if (!targets.empty()) {
		targetList.push_back(targets);
	}

	// 각 대상에 대해 메시지 처리
	for (std::vector<std::string>::iterator it = targetList.begin(); it != targetList.end(); ++it) {
		std::string target = *it;

		// 채널에 메시지 보내기
		if (!target.empty() && target.at(0) == '#') {
			Channel* channel = _server->findChannel(target);

			// 채널이 존재하지 않으면 조용히 넘어감 (NOTICE는 에러 응답을 보내지 않음)
			if (!channel)
				continue;

			// 채널에 메시지 전송 (자신 제외)
			std::string notification = ":" + _client->getId() + " NOTICE " + target + " :" + message;
			channel->broadcastMessage(notification, _client);
		}
		// 사용자에게 메시지 보내기
		else {
			Client *dest = _server->findClientByNick(target);
			if (!dest)
				continue; // NOTICE는 오류를 반환하지 않고 조용히 실패

			dest->write(":" + _client->getId() + " NOTICE " + target + " :" + message);
		}
	}
}