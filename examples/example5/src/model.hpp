#ifndef EXAMPLE_MODEL_HPP
#define EXAMPLE_MODEL_HPP

#include <set>
#include <string>
#include <iostream>


class player
{
public:
	virtual ~player(){};
	virtual void play(std::string) = 0;
	virtual void stop() = 0;
};

class audio_player
: public player
{
public:
	void play(std::string song)
	{
		std::cout << "audio player playing " << song << std::endl;
	}

	void stop()
	{
		std::cout << "audio player stopped" << std::endl;
	}
};

class video_player
: public player
{
public:
	void play(std::string video)
	{
		std::cout << "video player playing " << video << std::endl;
	}

	void stop()
	{
		std::cout << "video player stopped" << std::endl;
	}
};

class player_factory
{
	std::set<player*> m_players;
public:
	player* create(std::string type)
	{
		player* p = 0;
		if(type == "audio")
			p = new audio_player;
		else if(type == "video")
			p = new video_player;
		else
			return 0;

		m_players.insert(p);
		std::cout << "create " << type << std::endl;

		return p;
	}

	void destroy(player* p)
	{
		std::set<player*>::iterator iter = m_players.find(p);
		if(iter == m_players.end())
			return;

		std::cout << "destroy a player" << std::endl;

		m_players.erase(iter);
		delete p;
	}

	std::set<player*> all_player() const
	{
		return m_players;
	}
};

#endif
