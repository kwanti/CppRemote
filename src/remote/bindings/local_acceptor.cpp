// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/bindings/local_acceptor.hpp>
#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS

#include <remote/system_error.hpp>

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include <cstdio>


namespace remote
{
namespace bindings
{

local_acceptor::local_acceptor(io_service& ios)
: m_io_service(ios)
, m_strand(ios)
, m_acceptor(ios)
{}

void local_acceptor::listen(endpoint_type const& ep, handler const& hdl)
{
	m_strand.post(boost::bind(&this_type::do_listen, this, ep, hdl));
}

void local_acceptor::do_listen(endpoint_type aep, handler hdl)
{
	try
	{
		// remove socket file created by previous session
		std::remove(aep.path());

		asio_local::endpoint ep(aep.path());
		m_acceptor.open(ep.protocol());
		m_acceptor.set_option(asio_local::acceptor::receive_buffer_size(aep.in_buffer_size()));
		m_acceptor.set_option(asio_local::acceptor::send_buffer_size(aep.out_buffer_size()));
		m_acceptor.bind(ep);
		m_acceptor.listen();

		m_io_service.post(boost::bind(hdl, exception_ptr()));
	}
	catch(boost::system::system_error& error)
	{
		if(m_acceptor.is_open())
			m_acceptor.close();

		m_io_service.post(boost::bind(hdl, make_system_error(error.code())));
	}
}

void local_acceptor::close()
{
	m_strand.post(boost::bind(&this_type::do_close, this));
}

void local_acceptor::do_close()
{
	m_acceptor.close();
}

bool local_acceptor::is_listening() const
{
	return m_acceptor.is_open();
}

void local_acceptor::accept(asio_local::socket& socket, accept_handler const& hdl)
{
	m_strand.post(boost::bind(&this_type::do_accept, this, boost::ref(socket), hdl));
}

void local_acceptor::do_accept(asio_local::socket& socket, accept_handler hdl)
{
	m_acceptor.async_accept(socket, hdl);
}

}
}

#endif
