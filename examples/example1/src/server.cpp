#include "audio_player.hpp"
#include "remote_player.hpp"

#include <remote/server.hpp>
#include <remote/make_tcp_binding.hpp>
#include <iostream>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	audio_player player;

	remote::server server;
	server.bind<example::remote_player>(&player, "nice player");
	server.start(remote::make_tcp_binding(8888));

	std::cin.get();
	return 0;
}
