#include "info_service.hpp"
#include "remote_player.hpp"
#include "remote_ac.hpp"
#include "model.hpp"

#include <remote/server.hpp>
#include <remote/session.hpp>
#include <remote/global.hpp>
#include <remote/exception.hpp>
#include <remote/system_error.hpp>
#include <remote/remote_error.hpp>
#include <remote/make_basic_binding.hpp>
#include <remote/bindings/binary_serializer.hpp>
#include <remote/bindings/ipc_transport.hpp>

#include <iostream>

#ifdef REMOTE_USER_REGISTER_ARCHIVE
#include <remote/register_archive.ipp>
#endif

void on_start(remote::exception_ptr e)
{
	if(!e)
	{
		std::cout << "server started" << std::endl;
		return;
	}

	try	// handle error by rethrowing the exception
	{
		remote::rethrow_exception(e);
	}
	catch(remote::system_error& error)
	{
		std::cout << "system error: " << error.what() << std::endl;
	}
	catch(remote::remote_error& error)
	{
		std::cout << "remote error: " << error.what() << std::endl;
	}
	catch(std::exception& error)
	{
		std::cout << "other error: " << error.what() << std::endl;
	}
}

void on_accept(remote::exception_ptr e, remote::session& session)
{
	if(e)	// another way to handle error
	{
		std::cout << "error when accepting a session: " << e->what() << std::endl;
		return;
	}

	try
	{
		// get info service interface from the newly connected session.
		boost::shared_ptr<info_service> info = session.get<info_service>("info");
		std::cout << "accepted a session: " << info->name() << std::endl;
	}
	catch(std::exception& error)
	{
		std::cout << "error when get session info: " << error.what() << std::endl;
	}
}

int main()
{
	// set application io runner to run 2 threads
	remote::global::io_runner(2);

	// create target objects
	audio_player walkman;
	audio_player ipod;
	air_cond room_ac;

	// create multiple instances of remote services
	remote::server server;
	server.bind<remote_player>(&walkman, "walkman");
	server.bind<remote_player>(&ipod, "ipod");
	server.bind<remote_ac>(&room_ac, "room ac");

	// construct binding
	typedef remote::bindings::binary_serializer serializer;
	typedef remote::bindings::ipc_transport transport;
	remote::binding binding = remote::make_basic_binding<serializer, transport>("localhost");

	// setting event handlers and start the server
	server.set_accept_handler(on_accept);
	server.start(binding, on_start);

	std::cin.get();
	return 0;
}
