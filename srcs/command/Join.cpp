#include <Error.hpp>

#include "../../includes/Command.hpp"

void Join::execute()
{
	if (_client->getClientState() != REGISTERED) { // 451
		return _client->write(":" + _server->getServerName() + " " + Error::err_notregistered());
	}
	if (_args.size() < 2) { // 461
		return _client->write(":" + _server->getServerName() + " " + Error::err_needmoreparams(_client->getNickname(), _command));
	}
	if (_args.size() > 3) // 암 것도 안함.
		return ;
	
	if (_args.size() == 2 && _args[1] == "0") {	// channel 다 나감. (part 이용)
		std::vector<std::string> channel_names = _client->getChannelNames();
		t_command cmd;
		cmd._invalid_message = false;
		cmd._command = "PART";
		cmd._args.push_back("PART");
		for (std::vector<std::string>::iterator it = channel_names.begin(); it != channel_names.end(); it++)
		{
			cmd._args.push_back(*it);
		}
		Command * command = new Part();
		command->setCommand(_client, _server, cmd);
		command->execute();
		delete command;
	}
	// channel부분 잘라내기.
	this->makeChannelVec();
	if (_args.size() == 3) // key 존재
		this->makeKeyVec();


}

void Join::makeChannelVec()
{
	size_t pos = 0;
	std::string channel_str;
	for (size_t i = 0; i < _args[1].length(); i++)
	{
		if (_args[1][i] == ',')
		{
			if (pos < i)
			{
				channel_str = _args[1].substr(pos, i - pos);
				if (Channel::checkChannelNameFormat(channel_str))
					_channels.push_back(channel_str);
				else
				{	// 403
					_client->write(":" + _server->getServerName() + " " + Error::err_nosuchchannel(_client->getNickname(), channel_str));
				}
			}
			pos = i + 1;
		}
		else if (i == _args[1].length() -1 && pos <= i && _args[1][i] != ',')
		{
			channel_str = _args[1].substr(pos, i - pos + 1);
			if (Channel::checkChannelNameFormat(channel_str))
				_channels.push_back(channel_str);
			else
			{	// 403
				_client->write(":" + _server->getServerName() + " " + Error::err_nosuchchannel(_client->getNickname(), channel_str));
			}
		}
	}
}


void Join::makeKeyVec() // 수정해야함.
{
	size_t pos = 0;
	std::string key_str;
	for (size_t i = 0; i < _args[2].length(); i++)
	{
		if (i = 0 && _args[2][i] == ':') // 맨 앞에 : 이 왔을때에 대한 예외처리
		{
			i++;
			if (1 < _args[2].length() && _args[2][i] == ',')
			{
				pos = i + 1;
			}
			else 
				pos = i;
		}
		else if (_args[2][i] == ',')
		{
			if (pos < i)
			{
				key_str = _args[2].substr(pos, i - pos);
				// if (key_str.length() > 0)
					_keys.push_back(key_str);
			}
			pos = i + 1;
		}
		else if (i == _args[2].length() -1 && pos <= i && _args[1][i] != ',')
		{
			key_str = _args[2].substr(pos, i - pos + 1);
			_keys.push_back(key_str);
		}
	}
}

