#include "remote_player.hpp"

#include <remote/session.hpp>
#include <remote/make_tcp_binding.hpp>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	remote::session session;
	session.start(remote::make_tcp_binding("localhost", 8888));
	if(session.wait_for_ready() != remote::session::started)
		return -1;

	boost::shared_ptr<example::remote_player> player
		= session.get<example::remote_player>("nice player");

	player->play("nice song");
	player->stop();

	remote::future<int> f = player->rewind(2);
	f.get();

	return 0;
}
