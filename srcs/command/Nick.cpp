#include "../../includes/Command.hpp"

void Nick::execute()
{
	if (_args.empty())
		throw std::runtime_error(Error::err_needmoreparams(_client->getNickname(), "NICK"));

	// 닉네임 유효성 검사
	const std::string& newNick = _args[0];
	for (size_t j = 0; j < newNick.length(); ++j)
	{
		if (!isalnum(newNick[j]) && newNick[j] != '-' && newNick[j] != '_')
			throw std::runtime_error(Error::err_erroneousnickname(_client->getNickname(), newNick, "NICK"));
	}

	// 닉네임 중복 확인
	std::vector<std::string> clientNicknames = _server->getClientNicknames();
	if (std::find(clientNicknames.begin(), clientNicknames.end(), newNick) != clientNicknames.end())
		throw std::runtime_error(Error::err_nicknamealreadyuse(_client->getNickname(), newNick, "NICK"));

	// 기존 닉네임이 있으면 제거
	if (!_client->getNickname().empty())
		_server->removeClientNickname(_client->getNickname());

	// 새 닉네임 설정
	_client->setNickname(newNick);
	_server->addClientNickname(newNick);

	// 사용자 이름이 이미 설정되어 있고 현재 LOGIN 상태라면 등록 완료
	if (!_client->getUsername().empty() && _client->getClientState() == LOGIN)
	{
		_client->setId(_client->getNickname() + "!" + _client->getUsername() + "@" + _client->getHostname());
		_client->setClientState(REGISTERED);
		_server->sendMessage(_client, "001", ":Welcome to the Localnet IRC Network " + _client->getId());
	}
}