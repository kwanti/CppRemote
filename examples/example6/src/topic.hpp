#ifndef EXAMPLE_TOPIC_HPP
#define EXAMPLE_TOPIC_HPP

#include "remote_reader.hpp"
#include <boost/bind.hpp>
#include <string>
#include <set>


class topic
{
public:
	topic(std::string str)
	: m_about(str)
	{}

	std::string about() const { return m_about; }

	void subscribe(boost::shared_ptr<remote_reader> rdr)
	{
		m_readers.insert(rdr);
	}

	void unsubscribe(boost::shared_ptr<remote_reader> rdr)
	{
		m_readers.erase(rdr);
	}

	void post(std::string msg)
	{
		std::for_each(m_readers.begin(), m_readers.end(),
						boost::bind(&notify, _1, msg));
	}

private:
	std::string m_about;
	std::set<boost::shared_ptr<remote_reader> > m_readers;

	static void notify(boost::shared_ptr<remote_reader> rdr, std::string msg)
	{
		try
		{
			rdr->on_notify(msg);
		}
		catch(std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
	}
};

#endif
