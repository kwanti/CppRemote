// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/bindings/local_transport.hpp>
#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS

#include <remote/bindings/buffer_pool.hpp>
#include <remote/system_error.hpp>
#include <remote/global.hpp>

#include <boost/asio/read.hpp>
#include <boost/asio/write.hpp>
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

}


local_transport::local_transport(io_service& ios)
: m_state(stopped)
, m_io_service(ios)
, m_strand(ios)
, m_socket(ios)
, m_write_count(0)
, m_read_handler(0)
{}

local_transport::~local_transport()
{
	BOOST_ASSERT((m_state == stopped) || m_io_service.stopped());
}

void local_transport::accept(acceptor_type& ac, handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::do_accept, this, boost::ref(ac), hdl));
}

void local_transport::connect(endpoint_type const& ep, handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::do_connect, this, ep, hdl));
}

void local_transport::disconnect(handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::signal_disconnect, this, hdl));
}

void local_transport::write(buffer_ptr const& buf, handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::queue_write, this, buf, hdl));
}

void local_transport::read(read_handler const& hdl)
{
	m_strand.dispatch(boost::bind(&this_type::start_read, this, hdl));
}


void local_transport::do_accept(acceptor_type& ac, handler hdl)
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

void local_transport::on_accept(error_code ec, handler hdl)
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

void local_transport::do_connect(endpoint_type aep, handler hdl)
{
	if(m_state != stopped)
	{
		using namespace boost::asio::error;
		m_io_service.post(boost::bind(hdl, make_system_error(operation_aborted)));
		return;
	}

	m_state = starting;

	try
	{
		asio_local::endpoint ep(aep.path());
		m_socket.open(ep.protocol());
		m_socket.set_option(asio_local::acceptor::receive_buffer_size(aep.in_buffer_size()));
		m_socket.set_option(asio_local::acceptor::send_buffer_size(aep.out_buffer_size()));
		m_socket.async_connect(ep, m_strand.wrap(boost::bind(&this_type::on_connect, this, _1, hdl)));
	}
	catch(boost::system::system_error& error)
	{
		if(m_socket.is_open())
			m_socket.close();

		m_io_service.post(boost::bind(hdl, make_system_error(error.code())));
	}
}

void local_transport::on_connect(error_code ec, handler hdl)
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

		m_state = stopped;
		m_io_service.post(boost::bind(hdl, make_system_error(ec)));
		return;
	}

	m_state = started;
	m_io_service.post(boost::bind(hdl, exception_ptr()));
}

void local_transport::signal_disconnect(handler hdl)
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

void local_transport::do_disconnect(error_code ec, handler hdl)
{
	BOOST_ASSERT(hdl);
	BOOST_ASSERT(m_state == stopping);
	BOOST_ASSERT(!m_socket.is_open());

	if(!m_write_operations.empty() || (m_read_handler != 0))
	{
		boost::this_thread::yield();
		m_strand.post(boost::bind(&this_type::do_disconnect, this, ec, hdl));
		return;
	}

	BOOST_ASSERT(m_write_count == 0);
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

void local_transport::start_read(read_handler hdl)
{
	m_read_handler = hdl;
	do_read();
}

void local_transport::do_read()
{
	BOOST_ASSERT(!m_read_buffer);

	using namespace boost::asio;
	if(m_state != started)
	{
		m_io_service.post(boost::bind(m_read_handler,
			make_system_error(error::operation_aborted), buffer_ptr()));
		m_read_handler = 0;
		return;
	}

	std::size_t n = sizeof(header);
	m_read_buffer = global::buffer_pool().allocate();

	async_read(m_socket, m_read_buffer->prepare(n),
		m_strand.wrap(boost::bind(&this_type::on_read_size, this, _1, _2)));
}

void local_transport::on_read_size(error_code ec, std::size_t tx)
{
	BOOST_ASSERT(m_read_buffer);

	if(ec)
	{
		m_io_service.post(boost::bind(m_read_handler, make_system_error(ec), buffer_ptr()));
		m_read_buffer.reset();
		m_read_handler = 0;
		return;
	}

	if(tx != sizeof(header))
	{
		m_io_service.post(boost::bind(m_read_handler,
			make_system_error(boost::asio::error::operation_aborted), buffer_ptr()));

		m_read_buffer.reset();
		m_read_handler = 0;
		return;
	}
	m_read_buffer->commit(tx);

	header hdr;
	{
		std::istream is(m_read_buffer.get());
		is.read(reinterpret_cast<char*>(&hdr), sizeof(header));
	}

	if(!hdr.validate())
	{
		// how to handler this? discard everything in stream read buffer?
		m_io_service.post(boost::bind(m_read_handler,
			make_system_error(boost::asio::error::operation_aborted), buffer_ptr()));

		m_read_buffer.reset();
		m_read_handler = 0;
		return;
	}

	BOOST_ASSERT(m_read_buffer->size() == 0);

	boost::asio::async_read(m_socket, m_read_buffer->prepare(hdr.m_size),
		m_strand.wrap(boost::bind(&this_type::on_read_data, this, _1, _2)));
}

void local_transport::on_read_data(error_code ec, std::size_t tx)
{
	BOOST_ASSERT(m_read_buffer);

	if(ec)
	{
		m_io_service.post(boost::bind(m_read_handler, make_system_error(ec), buffer_ptr()));
		m_read_buffer.reset();
		m_read_handler = 0;
		return;
	}

	m_read_buffer->commit(tx);
	m_read_handler(exception_ptr(), m_read_buffer);
//	m_io_service.post(boost::bind(m_read_handler, exception_ptr(), m_read_buffer));
	m_read_buffer.reset();

	// start another read operation
	do_read();
}

void local_transport::queue_write(buffer_ptr buf, handler hdl)
{
	using namespace boost::asio;
	if(m_state != started)
	{
		m_io_service.post(boost::bind(hdl, make_system_error(error::operation_aborted)));
		return;
	}

	m_write_operations.push_back(write_operation(buf, hdl));
	do_write();
}

void local_transport::do_write()
{
	using namespace boost::asio;
	static const std::size_t max_write_count = 3;

	if(m_write_count || m_write_operations.empty())
		return;

	std::vector<boost::asio::const_buffer> buffers;
	for(; m_write_count < max_write_count && m_write_count < m_write_operations.size();
		++m_write_count)
	{
		write_operation& op = m_write_operations[m_write_count];
		buffers.push_back(boost::asio::buffer(&op.m_header, sizeof(op.m_header)));
		buffers.push_back(op.m_buffer->data());
	}

	async_write(m_socket, buffers,
		m_strand.wrap(boost::bind(&this_type::on_write, this, _1, _2)));
}

void local_transport::on_write(error_code ec, std::size_t)
{
	BOOST_ASSERT(m_write_operations.size() > 0);

	exception_ptr e;
	if(ec)
	{
		e = make_system_error(ec);
	}

	for(std::size_t i = 0; i < m_write_count; ++i)
	{
		write_operation& op = m_write_operations.front();
		op.m_buffer->consume(op.m_buffer->size());

		op.m_handler(e);
		//m_io_service.post(boost::bind(op.m_handler, e));

		m_write_operations.pop_front();
	}

	m_write_count = 0;
	do_write();
}

local_transport::header::header(boost::uint32_t size)
: m_magic(magic)
, m_reserve(0)
, m_size(size)
{}

bool local_transport::header::validate()
{
	if(m_magic == magic)
		return true;

	m_magic = convert(m_magic);
	if(m_magic != magic)
		return false;

	m_size = convert(m_size);
	return true;
}

}
}

#endif
