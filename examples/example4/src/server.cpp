
#include "remote_test.hpp"
#include <remote/server.hpp>
#include <remote/make_tcp_binding.hpp>
#include <boost/make_shared.hpp>
#include <iostream>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


class server_target
{
	std::string m_name;
public:
	server_target(std::string name)
	: m_name(name)
	{}

	void invoke(std::string msg)
	{
		std::cout << "invoke " << m_name << " from " << msg << std::endl;
	}
};

class test_target
{
	boost::shared_ptr<rmt> m_stored_ptr;
public:

	void set_rmt_ptr(boost::shared_ptr<rmt> r)
	{
		r->invoke("server");
		m_stored_ptr = r;
	}

	boost::shared_ptr<rmt> get_rmt_ptr()
	{
		return m_stored_ptr;
	}

	void invoke_all(std::vector<boost::shared_ptr<rmt> > v)
	{
		std::for_each(v.begin(), v.end(), invoke);
	}

	static void invoke(boost::shared_ptr<rmt>& r)
	{
		r->invoke("server");
	}
};

int main()
{
	remote::server server;
	server.bind<rmt>(boost::make_shared<server_target>("s1"), "s1");
	server.bind<rmt>(boost::make_shared<server_target>("s2"), "s2");
	server.bind<remote_test>(boost::make_shared<test_target>(), "test");

	server.start(remote::make_tcp_binding(8888));

	std::cin.get();
	return 0;
}
