// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/bindings/named_pipe_acceptor.hpp>
#ifdef BOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE

#include <remote/system_error.hpp>

#include <boost/thread/thread.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace bindings
{
namespace // helper
{

DWORD to_instances(DWORD connections)
{
	if(connections == 0 || connections >= PIPE_UNLIMITED_INSTANCES)
		return connections;

	return connections + 1;
}

HANDLE create_pipe(named_pipe_endpoint const& ep, DWORD flag, boost::system::error_code& ec)
{
	ec.clear();

	DWORD mode = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED | flag;
	DWORD max_inst = to_instances(ep.max_connections());
	DWORD obs = ep.out_buffer_size();
	DWORD ibs = ep.in_buffer_size();

	HANDLE new_pipe = ::CreateNamedPipeA(ep.path(), mode, 0, max_inst, obs, ibs, 0, 0);
	if(new_pipe == INVALID_HANDLE_VALUE)
		ec.assign(::GetLastError(), boost::asio::error::system_category);

	return new_pipe;
}


}

struct named_pipe_acceptor::accept_operation: public boost::noncopyable
{
	HANDLE m_pipe_handle;
	OVERLAPPED m_ovl;
	stream_handle& m_pipe;
	accept_handler m_handler;

	accept_operation(HANDLE handle, stream_handle& pipe, accept_handler const& hdl)
	: m_pipe_handle(handle), m_pipe(pipe), m_handler(hdl)
	{
		::memset(&m_ovl, 0, sizeof(m_ovl));
	}
};

named_pipe_acceptor::named_pipe_acceptor(io_service& ios)
: m_io_service(ios)
, m_strand(ios)
, m_acceptor(INVALID_HANDLE_VALUE)
, m_pending(0)
{}

void named_pipe_acceptor::listen(endpoint_type const& ep, handler const& hdl)
{
	BOOST_ASSERT(hdl);
	m_strand.post(boost::bind(&this_type::do_listen, this, ep, hdl));
}

void named_pipe_acceptor::do_listen(endpoint_type ep, handler hdl)
{
	error_code ec;
	pipe_listen(ep, ec);
	if(ec)
	{
		m_io_service.post(boost::bind(hdl, boost::make_shared<system_error>(ec)));
		return;
	}
	m_io_service.post(boost::bind(hdl, exception_ptr()));
}

void named_pipe_acceptor::pipe_listen(endpoint_type const& ep, error_code& ec)
{
	ec.clear();
	HANDLE new_pipe = create_pipe(ep, FILE_FLAG_FIRST_PIPE_INSTANCE, ec);
	if(ec)
		return;

	BOOST_ASSERT(new_pipe != INVALID_HANDLE_VALUE);

	m_acceptor = new_pipe;
	m_endpoint = ep;
	++m_pending;
}

void named_pipe_acceptor::accept(stream_handle& pipe, accept_handler const& hdl)
{
	BOOST_ASSERT(!pipe.is_open());
	m_strand.post(boost::bind(&this_type::do_accept, this, boost::ref(pipe), hdl));
}

void named_pipe_acceptor::do_accept(stream_handle& pipe, accept_handler hdl)
{
	++m_pending;
	pipe_async_accept(pipe, hdl);
}

void named_pipe_acceptor::pipe_async_accept(stream_handle& pipe, accept_handler const& hdl)
{
	error_code ec;

	// create a new pipe for next connection
	HANDLE new_pipe = m_acceptor;
	m_acceptor = create_pipe(m_endpoint, 0, ec);
	if(ec)
	{
		m_acceptor = new_pipe;
		m_io_service.post(boost::bind(hdl, ec));

		--m_pending;
		return;
	}
	BOOST_ASSERT(new_pipe != INVALID_HANDLE_VALUE);
	BOOST_ASSERT(m_acceptor != INVALID_HANDLE_VALUE);

	accept_operation_ptr op = boost::make_shared<accept_operation>(new_pipe, boost::ref(pipe), hdl);

	// start accept connection
	::ConnectNamedPipe(new_pipe, &op->m_ovl);

	DWORD error = ::GetLastError();
	if(error == ERROR_PIPE_CONNECTED)
	{
		op->m_pipe.assign(op->m_pipe_handle, ec);
		m_io_service.post(boost::bind(hdl, ec));
		--m_pending;
		return;
	}

	if(error != ERROR_IO_PENDING)
	{
		::CloseHandle(op->m_pipe_handle);
		ec.assign(error, boost::asio::error::system_category);
		m_io_service.post(boost::bind(hdl, ec));
		--m_pending;
		return;
	}

	m_strand.post(boost::bind(&this_type::pipe_poll_accept, this, op));
}

void named_pipe_acceptor::pipe_poll_accept(accept_operation_ptr op)
{
	BOOST_ASSERT(op);

	if(!HasOverlappedIoCompleted(&op->m_ovl))
	{
		poll_io_service();
		m_io_service.post(boost::bind(&this_type::pipe_poll_accept, this, op));
		return;
	}

	error_code ec;

	DWORD error = static_cast<DWORD>(op->m_ovl.Internal);
	if(m_endpoint == endpoint_type() || error == 0xC0000120 /* STATUS_CANCELLED */)
		error = ERROR_OPERATION_ABORTED;

	if(error)
	{
		::CloseHandle(op->m_pipe_handle);
		ec.assign(error, boost::asio::error::system_category);
	}

	op->m_pipe.assign(op->m_pipe_handle, ec);
	m_io_service.post(boost::bind(op->m_handler, ec));
	--m_pending;
}

void named_pipe_acceptor::close()
{
	m_strand.post(boost::bind(&this_type::do_close, this));
	while(is_listening())
	{
		poll_io_service();
	}
}

void named_pipe_acceptor::do_close()
{
	std::string path = m_endpoint.path();
	DWORD max_inst = to_instances(m_endpoint.max_connections());

	m_endpoint = endpoint_type();
	for(unsigned int i = 0; i< max_inst; ++i)
	{
		HANDLE handle = ::CreateFileA(path.c_str(), 0, 0, 0, OPEN_EXISTING, 0, 0);
		if(handle == INVALID_HANDLE_VALUE)
		{
			if(::GetLastError() == ERROR_PIPE_BUSY)
				break;	// no more listening pipe
		}
		else
		{
			::CloseHandle(handle);
		}
	}

	m_strand.post(boost::bind(&this_type::poll_close, this));
}

void named_pipe_acceptor::poll_close()
{
	if(m_pending > 1)
	{
		m_strand.post(boost::bind(&this_type::poll_close, this));
		return;
	}
	BOOST_ASSERT(m_pending == 1);

	::CloseHandle(m_acceptor);
	m_acceptor = INVALID_HANDLE_VALUE;
}

bool named_pipe_acceptor::is_listening() const
{
	return (m_acceptor != INVALID_HANDLE_VALUE);
}

void named_pipe_acceptor::poll_io_service() const
{
	try
	{
		if(!m_io_service.poll_one())
			boost::this_thread::sleep_for(boost::chrono::milliseconds(1));
	}
	catch(std::exception& e)
	{
		// log error here
		std::cerr << "io service throw: " << e.what() << std::endl;
	}
	catch(...)
	{
		// not to continue on unknown exception
		std::cerr << "io service throw !!!" << std::endl;
		exit(-1);
	}
}

}
}

#endif
