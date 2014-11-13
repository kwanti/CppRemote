
#include "session_info.hpp"
#include "info_service.hpp"
#include "remote_player.hpp"
#include "remote_ac.hpp"

#include <remote/session.hpp>
#include <remote/attribute.hpp>
#include <remote/exception.hpp>
#include <remote/global.hpp>
#include <remote/make_basic_binding.hpp>
#include <remote/bindings/binary_serializer.hpp>
#include <remote/bindings/ipc_transport.hpp>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif


void on_start(remote::exception_ptr e)
{
	// handle session start event here
}

void on_stop(remote::exception_ptr e)
{
	// handle session stop event here
}

void on_error(remote::exception_ptr e)
{
	// handle io error event here
}

int main()
{
	// set application io runner to run 2 threads
	remote::global::io_runner(2);

	// initialize
	session_info info("My Session");

	remote::session session;
	session.bind<info_service>(&info , "info");

	// construct binding
	typedef remote::bindings::binary_serializer serializer;
	typedef remote::bindings::ipc_transport transport;
	remote::binding binding = remote::make_basic_binding<serializer, transport>("localhost");

	// set event handler and start the session
	session.set_error_handler(on_error);
	session.start(binding, on_start);

	if(session.wait_for_ready() != remote::session::started)
		return -1;

	// get remote pointer
	boost::shared_ptr<remote_ac> ac = session.get<remote_ac>("room ac");
	boost::shared_ptr<remote_player> ipod = session.get<remote_player>("ipod");
	boost::shared_ptr<remote_player> walkman = session.get<remote_player>("walkman");

	// use ac
	ac->set_temperature(25);

	// use walkman
	walkman->play("jazz");		// this is 2 way call

	// switch to one way mode
	remote::attribute::set_one_way(ipod, true);
	ipod->play("pop rock");		// this is one way call

	// switch back to normal mode
	remote::attribute::set_one_way(ipod, false);
	ipod->stop();				// this is 2 way call

	// stop the session and set event handler
	session.stop(on_stop);

	return 0;
}
