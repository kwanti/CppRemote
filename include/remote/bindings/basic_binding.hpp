// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BASICBINDING_HPP__
#define __REMOTE_BINDINGS_BASICBINDING_HPP__

#include <remote/bindings/binding.hpp>
#include <remote/bindings/basic_acceptor.hpp>
#include <remote/bindings/basic_channel.hpp>
#include <remote/bindings/no_filter.hpp>

#include <boost/ref.hpp>
#include <boost/bind/bind.hpp>


namespace remote
{
namespace bindings
{

template<typename Serializer, typename Transport, typename Filter = no_filter>
class basic_binding : public binding
{
public:
	typedef basic_channel<Serializer, Transport, Filter> channel_type;
	typedef basic_acceptor<Serializer, Transport, Filter> acceptor_type;
	typedef typename Transport::endpoint_type endpoint_type;

public:
	explicit basic_binding(endpoint_type const& ep)
	: m_endpoint(ep)
	{}

	channel_ptr connect(io_service& ios, handler _handler)
	{
		boost::shared_ptr<channel_type> _channel
			= boost::make_shared<channel_type>(boost::ref(ios));

		_channel->connect(m_endpoint, _handler);
		return _channel;
	}

	acceptor_ptr listen(io_service& ios, handler _handler)
	{
		boost::shared_ptr<acceptor_type> _acceptor
			= boost::make_shared<acceptor_type>(boost::ref(ios));

		_acceptor->listen(m_endpoint, _handler);
		return _acceptor;
	}

private:
	endpoint_type m_endpoint;
};

}
}

#endif
