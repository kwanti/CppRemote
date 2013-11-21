#ifndef TUTORIAL_SESSION_INFO_HPP
#define TUTORIAL_SESSION_INFO_HPP
#include <string>

class session_info
{
public:
	session_info(std::string name): m_name(name) {}
	std::string name() const { return m_name; }

private:
	std::string m_name;
};

#endif // TUTORIAL_SESSION_INFO_HPP
