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
	
	if (_args.size() == 2 && _args[1] == "0") {
		// channel 다 나감.
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
					_channel.push_back(channel_str);
				else
				{	// 403
					_client->write(":" + _server->getServerName() + " " + Error::err_nosuchchannel(_client->getNickname(), channel_str));
				}
			}
			pos = i + 1;
		}
		else if (i == _args[1].length() -1 && pos <= i)
		{
			channel_str = _args[1].substr(pos, i - pos + 1);
			if (Channel::checkChannelNameFormat(channel_str))
				_channel.push_back(channel_str);
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
		if (_args[1][i] == ',')
		{
			if (pos < i)
			{
				key_str = _args[2].substr(pos, i - pos);
				if (Channel::checkChannelNameFormat(key_str))
					_channel.push_back(key_str);
				else
				{	// 403
					_client->write(":" + _server->getServerName() + " " + Error::err_nosuchchannel(_client->getNickname(), key_str));
				}
			}
			pos = i + 1;
		}
		else if (i == _args[2].length() -1 && pos <= i)
		{
			key_str = _args[2].substr(pos, i - pos + 1);
			if (Channel::checkChannelNameFormat(key_str))
				_channel.push_back(key_str);
			else
			{	// 403
				_client->write(":" + _server->getServerName() + " " + Error::err_nosuchchannel(_client->getNickname(), key_str));
			}
		}
	}
}

