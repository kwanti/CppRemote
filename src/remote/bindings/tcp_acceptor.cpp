// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/bindings/tcp_acceptor.hpp>
#include <remote/system_error.hpp>

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace bindings
{

tcp_acceptor::tcp_acceptor(io_service& ios)
: m_io_service(ios)
, m_strand(ios)
, m_acceptor(ios)
{}

void tcp_acceptor::listen(endpoint_type const& ep, handler const& hdl)
{
	BOOST_ASSERT(hdl);
	ep.resolve(m_io_service,
		m_strand.wrap(boost::bind(&this_type::on_resolve, this, _1, _2, _3, hdl)));
}

void tcp_acceptor::on_resolve(error_code ec, iterator iter, resolver_ptr resolver, handler hdl)
{
	BOOST_ASSERT(hdl);

	if(ec)
	{
		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	try
	{
		asio_tcp::endpoint ep = *iter;
		m_acceptor.open(ep.protocol());
		m_acceptor.set_option(asio_tcp::acceptor::reuse_address(true));
		m_acceptor.bind(ep);
		m_acceptor.listen();

		m_io_service.post(boost::bind(hdl, exception_ptr()));
	}
	catch(error_code& e)
	{
		if(m_acceptor.is_open())
			m_acceptor.close();

		// retry with next endpoint?
		m_io_service.post(boost::bind(hdl, make_system_error(e)));
	}
}

void tcp_acceptor::close()
{
	m_strand.post(boost::bind(&this_type::do_close, this));
}

void tcp_acceptor::do_close()
{
	m_acceptor.close();
}

bool tcp_acceptor::is_listening() const
{
	return m_acceptor.is_open();
}

void tcp_acceptor::accept(asio_tcp::socket& socket, accept_handler const& hdl)
{
	m_strand.post(boost::bind(&this_type::do_accept, this, boost::ref(socket), hdl));
}

void tcp_acceptor::do_accept(asio_tcp::socket& socket, accept_handler hdl)
{
	m_acceptor.async_accept(socket, hdl);
}

}
}
