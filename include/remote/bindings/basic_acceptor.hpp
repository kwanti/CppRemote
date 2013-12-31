// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BASICACCEPTOR_HPP__
#define __REMOTE_BINDINGS_BASICACCEPTOR_HPP__

#include <remote/bindings/acceptor.hpp>
#include <remote/bindings/basic_channel.hpp>

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/bind/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace bindings
{

template<typename Serializer, typename Transport, typename Filter>
class basic_acceptor : public acceptor
{
public:
	typedef typename Transport::endpoint_type endpoint_type;
	typedef typename Transport::acceptor_type acceptor_type;
	typedef basic_channel<Serializer, Transport, Filter> channel_type;

public:
	explicit basic_acceptor(io_service& ios)
	: m_io_service(ios)
	, m_acceptor(ios)
	{}

	void close()
	{
		m_acceptor.close();
	}

	void listen(endpoint_type ep, handler _handler)
	{
		m_acceptor.listen(ep, _handler);
	}

	bool is_listening() const
	{
		return m_acceptor.is_listening();
	}

	channel_ptr accept(handler _handler)
	{
		boost::shared_ptr<channel_type> _channel
			= boost::make_shared<channel_type>(boost::ref(m_io_service));

		_channel->accept(m_acceptor, _handler);
		return _channel;
	}

private:
	io_service& m_io_service;
	acceptor_type m_acceptor;
};

}
}

#endif
