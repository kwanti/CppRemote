#ifndef TUTORIAL_MODEL_HPP
#define TUTORIAL_MODEL_HPP
#include <string>
#include <iostream>


class audio_player
{
public:
	void play(std::string song)
	{
		std::cout << "playing " << song << std::endl;
		m_playing = true;
	}

	void stop()
	{
		std::cout << "stop playing " << std::endl;
		m_playing = false;
	}

private:
	bool m_playing;
};


class air_cond
{
public:
	void set_temperature(int t)
	{
		std::cout << "temperature now is " << t << std::endl;
		m_temperature = t;
	}

	int get_temperature() const
	{
		return m_temperature;
	}

private:
	int m_temperature;
};

#endif // TUTORIAL_MODEL_HPP
