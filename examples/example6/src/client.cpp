
#include "remote_topic.hpp"
#include <remote/session.hpp>
#include <remote/make_tcp_binding.hpp>
#include <iostream>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


class reader
{
public:
	void on_notify(std::string msg)
	{
		std::cout << msg << std::endl;
	}
};

int main()
{
	remote::session session;
	session.start(remote::make_tcp_binding("127.0.0.1", 8888));

	std::string name;
	std::cout << "Enter your name: ";
	std::getline(std::cin, name);

	if(session.wait_for_ready() != remote::session::started)
		return -1;

	boost::shared_ptr<remote_reader> rdr
		= session.bind<remote_reader>(boost::make_shared<reader>());

	boost::shared_ptr<remote_topic> tp
		= session.get<remote_topic>("cppr");

	tp->subscribe(rdr);
	std::cout << "*** \"" << tp->about() << "\" ***" << std::endl;

	std::string input;
	while(input != "bye")
	{
		std::getline(std::cin, input);
		tp->post(name + ": " + input);
	}

	tp->unsubscribe(rdr);
	std::cout << "*** unsubscribed ***" << std::endl;

	std::cin.get();
	return 0;
}
