#include "service.hpp"
#include "remote_service.hpp"

#include <remote/server.hpp>
#include <remote/make_tcp_binding.hpp>
#include <boost/make_shared.hpp>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	remote::server server;

	server.bind<remote_service>(boost::make_shared<service>(), "test");
	server.start(remote::make_tcp_binding("localhost", 9999));

	std::cin.get();
	return 0;
}
