#include "remote_factory.hpp"
#include <remote/session.hpp>
#include <remote/make_tcp_binding.hpp>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


int main()
{
	remote::session session;
	session.start(remote::make_tcp_binding("127.0.0.1", 8888));
	if(session.wait_for_ready() != remote::session::started)
		return -1;

	boost::shared_ptr<remote_factory> factory = session.get<remote_factory>("factory");

	remote_player* audio_player = factory->create("audio");
	remote_player* video_player = factory->create("video");
	remote_player* null_player = factory->create("other");

	BOOST_VERIFY(!null_player);
	audio_player->play("nice song");
	video_player->play("nice movie");

	std::set<remote_player*> players = factory->all_player();
	std::for_each(players.begin(), players.end(), boost::bind(&remote_player::stop, _1));

	factory->destroy(audio_player);
	factory->destroy(video_player);

    session.release(audio_player);
    session.release(video_player);
}
