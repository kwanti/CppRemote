#include "factory_adaptor.hpp"
#include "remote_factory.hpp"
#include <remote/server.hpp>
#include <remote/make_tcp_binding.hpp>
#include <iostream>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	player_factory factory;
	remote::server server;

	factory_adaptor adaptor(server, factory);
	server.bind<remote_factory>(&adaptor, "factory");

	server.start(remote::make_tcp_binding(8888));

	std::cin.get();
	return 0;
}
