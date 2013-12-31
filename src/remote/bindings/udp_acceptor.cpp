// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/bindings/udp_acceptor.hpp>
#include <remote/bindings/buffer.hpp>
#include <remote/system_error.hpp>

#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace bindings
{
namespace // helper
{

exception_ptr make_system_error(boost::system::error_code const& ec)
{
	return boost::make_shared<system_error>(ec);
}

boost::uint16_t convert(boost::uint16_t value)
{
	union
	{
		boost::uint16_t v;
		struct
		{
			unsigned char b0;
			unsigned char b1;
		} b;
	} u;

	u.v = value;
	std::swap(u.b.b0, u.b.b1);

	return u.v;
}

boost::uint32_t convert(boost::uint32_t value)
{
	union
	{
		boost::uint32_t v;
		struct
		{
			unsigned char b0;
			unsigned char b1;
			unsigned char b2;
			unsigned char b3;
		} b;
	} u;

	u.v = value;
	std::swap(u.b.b0, u.b.b3);
	std::swap(u.b.b1, u.b.b2);

	return u.v;
}


struct header
{
	enum { magic = 0x3a29 };
	boost::uint16_t m_magic;
	boost::uint16_t m_reserve;
	boost::uint32_t m_data;

	header()
	{}

	header(boost::uint32_t data)
	: m_magic(magic)
	, m_reserve(0)
	, m_data(data)
	{}

	bool validate()
	{
		if(m_magic == magic)
			return true;

		m_magic = convert(m_magic);
		if(m_magic != magic)
			return false;

		m_data = convert(m_data);
		return true;
	}
};

}

struct udp_acceptor::accept_operation
{
	buffer m_buffer;
	asio_udp::endpoint m_endpoint;
	asio_udp::socket& m_socket;
	accept_handler m_handler;

	accept_operation(asio_udp::socket& socket, accept_handler const& hdl)
	: m_socket(socket)
	, m_handler(hdl)
	{}
};

udp_acceptor::udp_acceptor(io_service& ios)
: m_io_service(ios)
, m_strand(ios)
, m_acceptor(ios)
{}

void udp_acceptor::listen(udp_endpoint const& ep, handler const& hdl)
{
	BOOST_ASSERT(hdl);

	ep.resolve(m_io_service,
		m_strand.wrap(boost::bind(&this_type::on_resolve, this, _1, _2, _3, hdl)));
}

void udp_acceptor::on_resolve(error_code ec, iterator iter, resolver_ptr resolver, handler hdl)
{
	BOOST_ASSERT(hdl);

	if(ec)
	{
		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	try
	{
		asio_udp::endpoint ep = *iter;
		m_acceptor.open(ep.protocol());
		m_acceptor.bind(ep);

		m_io_service.post(boost::bind(hdl, exception_ptr()));
	}
	catch(boost::system::system_error& e)
	{
		if(m_acceptor.is_open())
			m_acceptor.close();

		// retry with next endpoint?
		m_io_service.post(boost::bind(hdl, make_system_error(e.code())));
	}
}

void udp_acceptor::close()
{
	m_strand.post(boost::bind(&this_type::do_close, this));
}

void udp_acceptor::do_close()
{
	error_code ec;
	m_acceptor.shutdown(boost::asio::socket_base::shutdown_both, ec);
	m_acceptor.close(ec);
}

bool udp_acceptor::is_listening() const
{
	return m_acceptor.is_open();
}

void udp_acceptor::accept(asio_udp::socket& socket, accept_handler const& hdl)
{
	BOOST_ASSERT(hdl);
	m_strand.post(boost::bind(&this_type::do_accept, this, boost::ref(socket), hdl));
}

void udp_acceptor::do_accept(asio_udp::socket& socket, accept_handler hdl)
{
	udp_async_accept(socket, hdl);
}

void udp_acceptor::udp_async_accept(asio_udp::socket& socket, accept_handler const& hdl)
{
	accept_operation_ptr op = boost::make_shared<accept_operation>(boost::ref(socket), hdl);

	m_acceptor.async_receive_from(op->m_buffer.prepare(mtu_size), op->m_endpoint,
		m_strand.wrap(boost::bind(&this_type::udp_on_accept, this, _1, _2, op)));
}

void udp_acceptor::udp_retry_accept(accept_operation_ptr op)
{
	if(op->m_socket.is_open())
		op->m_socket.close();

	op->m_buffer.consume(op->m_buffer.size());
	op->m_endpoint = asio_udp::endpoint();

	m_acceptor.async_receive_from(op->m_buffer.prepare(mtu_size), op->m_endpoint,
		m_strand.wrap(boost::bind(&this_type::udp_on_accept, this, _1, _2, op)));
}

void udp_acceptor::udp_on_accept(error_code ec, std::size_t tx, accept_operation_ptr op)
{
	// check read status
	if(ec)
	{
		m_io_service.post(boost::bind(op->m_handler, ec));
		return;
	}

	if(tx < sizeof(header))
	{
		// invalid connection request. just ignore this message and try to accept next.
		udp_retry_accept(op);
		return;
	}
	op->m_buffer.commit(tx);


	// read header
	header hdr;
	{
		std::istream is(&op->m_buffer);
		is.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	}


	// validate header and remote endpoint
	if(!hdr.validate() || (hdr.m_data != op->m_endpoint.port()))
	{
		udp_retry_accept(op);
		return;
	}


	// connect accepted socket
	op->m_socket.connect(op->m_endpoint, ec);
	if(ec)
	{
		m_io_service.post(boost::bind(op->m_handler, ec));
		return;
	}

	BOOST_ASSERT(op->m_socket.is_open());
	BOOST_ASSERT(hdr.m_magic == header::magic);	// to make sure we can reuse this


	// send the connected socket port to remote
	hdr.m_data = op->m_socket.local_endpoint().port();
	tx = op->m_socket.send(boost::asio::buffer(&hdr, sizeof(hdr)), 0, ec);
	if(ec)
	{
		udp_retry_accept(op);
		return;
	}
	BOOST_ASSERT(tx == sizeof(hdr));


	// call handler with no error
	m_io_service.post(boost::bind(op->m_handler, ec));
}

}
}
