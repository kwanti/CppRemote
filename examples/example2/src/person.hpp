#ifndef EXAMPLE_PERSON_HPP
#define EXAMPLE_PERSON_HPP

#include <string>
#include <iostream>


class person
{
public:
	std::string m_name;
	int m_age;

	person()
	: m_age(0)
	{}

	person(std::string name, int age)
	: m_name(name), m_age(age)
	{}

	void info() const
	{
		std::cout << m_name << " (" << m_age << ")" << std::endl;
	}

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const)
	{
		ar & m_name & m_age;
	}
};

#endif
