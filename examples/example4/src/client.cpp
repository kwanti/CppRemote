
#include "remote_test.hpp"
#include <remote/session.hpp>
#include <remote/make_tcp_binding.hpp>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


class client_target
{
	std::string m_name;
public:
	client_target(std::string name)
	: m_name(name)
	{}

	void invoke(std::string msg)
	{
		std::cout << "invoke " << m_name << " from " << msg << std::endl;
	}
};


int main()
{
	remote::session session;

	client_target tc1("c1");
	boost::shared_ptr<client_target> tc2 = boost::make_shared<client_target>("c2");

	boost::shared_ptr<rmt> c1 = session.bind<rmt>(&tc1);
	boost::shared_ptr<rmt> c2 = session.remote_cast<rmt>(tc2);

	session.start(remote::make_tcp_binding("localhost", 8888));
	if(session.wait_for_ready() != remote::session::started)
		return -1;

	boost::shared_ptr<rmt> s1 = session.get<rmt>("s1");
	boost::shared_ptr<rmt> s2 = session.get<rmt>("s2");
	boost::shared_ptr<remote_test> test = session.get<remote_test>("test");

	test->set_rmt_ptr(c1);
	boost::shared_ptr<rmt> rc1 = test->get_rmt_ptr();
	rc1->invoke("client");

	test->set_rmt_ptr(s1);
	boost::shared_ptr<rmt> rs1 = test->get_rmt_ptr();
	rs1->invoke("client");

	std::vector<boost::shared_ptr<rmt> > v;
	v.push_back(c1);
	v.push_back(s1);
	v.push_back(c2);
	v.push_back(s2);

	test->invoke_all(v);
	return 0;
}
