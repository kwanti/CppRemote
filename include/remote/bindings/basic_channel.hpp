// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BASICCHANNEL_HPP__
#define __REMOTE_BINDINGS_BASICCHANNEL_HPP__

#include <remote/bindings/channel.hpp>
#include <remote/bindings/buffer.hpp>
#include <remote/archive_error.hpp>

#include <boost/bind/bind.hpp>


namespace remote
{
namespace bindings
{

template<typename Serializer, typename Transport, typename Filter>
class basic_channel
: public channel
{
public:
	typedef typename Transport::endpoint_type endpoint_type;
	typedef typename Transport::acceptor_type acceptor_type;

public:
	explicit basic_channel(io_service& ios)
	: m_transport(ios)
	, m_call_handler(0)
	{}

	void accept(acceptor_type& _acceptor, handler const& _handler)
	{
		m_transport.accept(_acceptor, _handler);
	}

	void connect(endpoint_type ep, handler const& _handler)
	{
		m_transport.connect(ep, _handler);
	}

	void disconnect(handler const& _handler)
	{
		m_transport.disconnect(_handler);
	}

	state_t state() const
	{
		return static_cast<state_t>(m_transport.state());
	}

	void send(call_ptr const& _call, handler const& _handler)
	{
		BOOST_ASSERT(_call);
		BOOST_ASSERT(_handler);

		try
		{
			Filter filter;
			Serializer serializer;
			m_transport.write(filter.output(serializer.serialize(_call)), _handler);
		}
		catch(archive_error& e)
		{
			_handler(remote::make_exception_ptr(e));
		}
	}

	void receive(call_handler const& _handler)
	{
		BOOST_ASSERT(_handler);
		BOOST_ASSERT(m_call_handler == 0);

		m_call_handler = _handler;
		m_transport.read(boost::bind(&this_type::on_receive, this, _1, _2));
	}

private:
	typedef basic_channel<Serializer, Transport, Filter> this_type;

private:
	Transport m_transport;
	call_handler m_call_handler;

private:

	void on_receive(exception_ptr _exception, buffer_ptr _buffer)
	{
		if(_exception)
		{
			m_call_handler(_exception, call_ptr());
			m_call_handler = 0;
			return;
		}

		try
		{
			Filter filter;
			Serializer serializer;
			m_call_handler(exception_ptr(), serializer.deserialize(filter.input(_buffer)));
		}
		catch(archive_error& e)
		{
			m_call_handler(remote::make_exception_ptr(e), call_ptr());
		}
	}
};

}
}

#endif
