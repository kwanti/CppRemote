
#include "topic.hpp"
#include "remote_topic.hpp"
#include <remote/server.hpp>
#include <remote/make_tcp_binding.hpp>
#include <iostream>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	std::string about("Sharing CppRemote techniques");
	topic tp(about);

	remote::server server;
	server.bind<remote_topic>(&tp, "cppr");

	server.start(remote::make_tcp_binding(8888));
	std::cout << "serving topic \"" << about << "\"" << std::endl;

	std::cin.get();
	return 0;
}
