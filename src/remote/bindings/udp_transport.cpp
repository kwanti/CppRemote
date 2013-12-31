// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/bindings/udp_transport.hpp>
#include <remote/bindings/buffer_pool.hpp>
#include <remote/system_error.hpp>
#include <remote/global.hpp>

#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/make_shared.hpp>


namespace remote
{
namespace bindings
{
namespace // helper
{

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



struct udp_transport::connect_operation
{
	buffer m_buffer;
	asio_udp::endpoint m_endpoint;
	impl_handler m_handler;

	connect_operation(impl_handler const& h)
	: m_handler(h)
	{}
};

struct udp_transport::write_operation
{
	header m_header;
	buffer_ptr m_buffer;
	handler m_handler;

	write_operation(buffer_ptr const& buf, handler const& h)
	: m_header(static_cast<boost::uint32_t>(buf->size()))
	, m_buffer(buf)
	, m_handler(h)
	{}
};


#if defined(BOOST_WINDOWS) || defined(__CYGWIN__)
const boost::system::error_code
udp_transport::more_data_available(ERROR_MORE_DATA,
				boost::asio::error::system_category);
#else
const boost::system::error_code udp_transport::more_data_available;
#endif


udp_transport::udp_transport(io_service& ios)
: m_state(stopped)
, m_io_service(ios)
, m_strand(ios)
, m_socket(ios)
, m_timer(ios)
, m_connect_timeout(static_cast<boost::chrono::milliseconds::rep>(default_connect_timeout))
, m_read_handler(0)
{}

udp_transport::~udp_transport()
{
	BOOST_ASSERT((m_state == stopped) || m_io_service.stopped());
}

udp_transport::endpoint_type udp_transport::local_endpoint() const
{
	error_code ec;
	return endpoint_type(m_socket.local_endpoint(ec));
}

udp_transport::endpoint_type udp_transport::remote_endpoint() const
{
	error_code ec;
	return endpoint_type(m_socket.remote_endpoint(ec));
}

void udp_transport::accept(acceptor_type& ac, handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::do_accept, this, boost::ref(ac), hdl));
}

void udp_transport::connect(endpoint_type const& ep, handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::do_connect, this, ep, hdl));
}

void udp_transport::disconnect(handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::signal_disconnect, this, hdl));
}

void udp_transport::write(buffer_ptr const& buf, handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::do_write, this, buf, hdl));
}

void udp_transport::read(read_handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::start_read, this, hdl));
}

void udp_transport::do_accept(acceptor_type& ac, handler hdl)
{
	if(m_state != stopped)
	{
		using namespace boost::asio::error;
		m_io_service.post(boost::bind(hdl, make_system_error(operation_aborted)));
		return;
	}

	m_state = starting;
	m_strand.post(boost::bind(&acceptor_type::accept, &ac, boost::ref(m_socket),
		m_strand.wrap(boost::bind(&this_type::on_accept, this, _1, hdl))));
}

void udp_transport::on_accept(error_code ec, handler hdl)
{
	if(m_state != starting)
	{
		if(!ec)
			ec = boost::asio::error::operation_aborted;

		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	if(ec)
	{
		if(m_socket.is_open())
			m_socket.close();

		m_state = stopped;
		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	m_state = started;
	m_io_service.post(boost::bind(hdl, exception_ptr()));
}

void udp_transport::do_connect(endpoint_type ep, handler hdl)
{
	if(m_state != stopped)
	{
		using namespace boost::asio::error;
		m_io_service.post(boost::bind(hdl, make_system_error(operation_aborted)));
		return;
	}

	m_state = starting;

	// how to cancel resolve operation when try to disconnect?
	ep.resolve(m_io_service,
		m_strand.wrap(boost::bind(&this_type::on_resolve, this, _1, _2, _3, hdl)));
}

void udp_transport::on_resolve(error_code ec, iterator iter, resolver_ptr rsv, handler hdl)
{
	if(m_state != starting)
	{
		if(!ec)
			ec = boost::asio::error::operation_aborted;

		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	if(ec)
	{
		m_state = stopped;
		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	udp_connect(*iter, m_strand.wrap(boost::bind(&this_type::on_connect, this, _1, iter, hdl)));
}

void udp_transport::on_connect(error_code ec, iterator iter, handler hdl)
{
	BOOST_ASSERT(hdl);

	if(m_state != starting)
	{
		if(m_socket.is_open())
			m_socket.close();

		if(!ec)
			ec = boost::asio::error::operation_aborted;

		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	if(ec)
	{
		if(m_socket.is_open())
			m_socket.close();

		using namespace boost::asio::error;
		if((ec != operation_aborted) && (ec != timed_out) && ((++iter != iterator())))
		{
			// retry with next endpoint
			udp_connect(*iter,
				m_strand.wrap(boost::bind(&this_type::on_connect, this, _1, iter, hdl)));
		}
		else
		{
			m_state = stopped;
			m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		}
		return;
	}

	m_state = started;
	m_io_service.post(boost::bind(hdl, exception_ptr()));
}

void udp_transport::udp_connect(asio_udp::endpoint ep, impl_handler hdl)
{
	try
	{
		// should get all local interface endpoint with same protocol as remote endpoint
		// and retry for each resolved local endpoint until connection success or all fail.
		// asio does not implement query interface.  the work around (less robust) is to
		// query endpoint with the hostname.
		asio_udp::endpoint local(ep.protocol(), 0);

		m_socket.open(ep.protocol());	// check if user has already open this socket
		m_socket.bind(local);			// which interface is this binding to?

		// assume bind api have selected a port number for us
		BOOST_ASSERT(m_socket.local_endpoint().port() != 0);

		header hdr(m_socket.local_endpoint().port());
		m_socket.send_to(boost::asio::buffer(&hdr, sizeof(hdr)), ep);

		// set connect timeout timer
		m_timer.expires_from_now(m_connect_timeout);
		m_timer.async_wait(m_strand.wrap(boost::bind(&this_type::on_udp_connect_timeout, this, _1, hdl)));

		connect_operation_ptr op = boost::make_shared<connect_operation>(hdl);

		m_socket.async_receive_from(op->m_buffer.prepare(mtu_size), op->m_endpoint,
			m_strand.wrap(boost::bind(&this_type::on_udp_connect, this, _1, _2, op)));
	}
	catch(boost::system::system_error& e)
	{
		hdl(e.code());
	}
}

void udp_transport::on_udp_connect_timeout(error_code ec, impl_handler hdl)
{
	if(ec == boost::asio::error::operation_aborted)
	{
		// timer has been canceled
		return;
	}
	else if(ec)
	{
		hdl(ec);
		return;
	}

	// timeout
	hdl(boost::asio::error::timed_out);
}

void udp_transport::on_udp_connect(error_code ec, std::size_t tx, connect_operation_ptr op)
{
	if(!m_timer.cancel())
	{
		// timer already timeout
		return;
	}

	if(ec)
	{
		op->m_handler(ec);
		return;
	}

	if(tx < sizeof(header))
	{
		// invalid message
		op->m_handler(boost::asio::error::connection_refused);
		return;
	}
	op->m_buffer.commit(tx);

	// read header
	header hdr;
	{
		std::istream is(&op->m_buffer);
		is.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	}
	op->m_buffer.consume(sizeof(hdr));

	// validate header
	if(!hdr.validate() || hdr.m_data != op->m_endpoint.port())
	{
		op->m_handler(boost::asio::error::connection_refused);
		return;
	}

	m_socket.connect(op->m_endpoint, ec);
	if(ec)
	{
		op->m_handler(ec);
		return;
	}

	BOOST_ASSERT(m_socket.remote_endpoint() == op->m_endpoint);

	op->m_handler(ec);
}

void udp_transport::signal_disconnect(handler hdl)
{
	BOOST_ASSERT(hdl);

	if((m_state == stopped) || (m_state == stopping))
	{
		using namespace boost::asio::error;
		m_io_service.post(boost::bind(hdl, make_system_error(operation_aborted)));
		return;
	}

	// signal stopping
	m_state = stopping;

	error_code ec;
	m_socket.shutdown(boost::asio::socket_base::shutdown_both, ec);
	m_socket.close(ec);

	// delay the shutdown to let the pending io operations to abort
	m_strand.post(boost::bind(&this_type::do_disconnect, this, ec, hdl));
}

void udp_transport::do_disconnect(error_code ec, handler hdl)
{
	BOOST_ASSERT(hdl);
	BOOST_ASSERT(m_state == stopping);
	BOOST_ASSERT(!m_socket.is_open());

	if(m_read_handler != 0)
	{
		boost::this_thread::yield();
		m_strand.post(boost::bind(&this_type::do_disconnect, this, ec, hdl));
		return;
	}

	BOOST_ASSERT(!m_read_buffer);
	BOOST_ASSERT(m_read_handler == 0);

	m_state = stopped;

	if(ec)
	{
		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	m_io_service.post(boost::bind(hdl, exception_ptr()));
}

void udp_transport::start_read(read_handler hdl)
{
	m_read_handler = hdl;
	do_read();
}

void udp_transport::do_read()
{
	BOOST_ASSERT(m_read_handler);

	using namespace boost::asio;
	if(m_state != started)
	{
		m_io_service.post(boost::bind(m_read_handler,
			make_system_error(error::operation_aborted), buffer_ptr()));
		m_read_handler = 0;
		return;
	}

	m_read_buffer = global::buffer_pool().allocate();
	BOOST_ASSERT(m_read_buffer->size() == 0);

	m_socket.async_receive(m_read_buffer->prepare(mtu_size),
		m_strand.wrap(boost::bind(&this_type::on_read, this, _1, _2)));
}

void udp_transport::on_read(error_code ec, std::size_t tx)
{
	if(ec)
	{
		using namespace boost::asio::error;
		if((ec == more_data_available) || (ec == message_size))
		{
			// message too large, just discard it
			do_read();
			return;
		}

		m_io_service.post(boost::bind(m_read_handler, make_system_error(ec), buffer_ptr()));
		m_read_buffer.reset();
		m_read_handler = 0;
		return;
	}

	if(tx < sizeof(header))
	{
		// invalid message, just discard it
		do_read();
		return;
	}
	m_read_buffer->commit(tx);

	header hdr;
	{
		std::istream is(m_read_buffer.get());
		is.read(reinterpret_cast<char*>(&hdr), sizeof(hdr));
	}

	if(!hdr.validate() || hdr.m_data != m_read_buffer->size())
	{
		// invalid message, just discard it
		do_read();
		return;
	}

	m_read_handler(exception_ptr(), m_read_buffer);

	// start another read operation
	do_read();
}

void udp_transport::do_write(buffer_ptr buf, handler hdl)
{
	using namespace boost::asio;
	if(m_state != started)
	{
		m_io_service.post(boost::bind(hdl, make_system_error(error::operation_aborted)));
		return;
	}

	if(buf->size() > max_write_size)
	{
		m_io_service.post(boost::bind(hdl, make_system_error(error::message_size)));
		return;
	}

	write_operation_ptr op = boost::make_shared<write_operation>(buf, hdl);

	std::vector<const_buffer> buffers;
	buffers.push_back(boost::asio::buffer(&op->m_header, sizeof(op->m_header)));
	buffers.push_back(op->m_buffer->data());

	m_socket.async_send(buffers,
		m_strand.wrap(boost::bind(&this_type::on_write, this, _1, _2, op)));
}

void udp_transport::on_write(error_code ec, std::size_t, write_operation_ptr op)
{
	if(ec)
	{
		op->m_handler(make_system_error(ec));
		return;
	}

	op->m_handler(exception_ptr());
}

}
}
