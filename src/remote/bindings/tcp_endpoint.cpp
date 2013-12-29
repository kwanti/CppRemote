// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/bindings/tcp_endpoint.hpp>

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace bindings
{

tcp_endpoint::tcp_endpoint(std::string const& host, std::string const& svc)
: m_query(host, svc)
{}

tcp_endpoint::tcp_endpoint(std::string const& host, int port)
: m_query(host, boost::lexical_cast<std::string>(port))
{}

tcp_endpoint::tcp_endpoint(asio_tcp const& version, std::string const& svc)
: m_query(version, svc)
{}

tcp_endpoint::tcp_endpoint(asio_tcp const& version, int port)
: m_query(version, boost::lexical_cast<std::string>(port))
{}

tcp_endpoint::tcp_endpoint(std::string const& svc)
: m_query(svc)
{}

tcp_endpoint::tcp_endpoint(int port)
: m_query(boost::lexical_cast<std::string>(port))
{}

void tcp_endpoint::resolve(io_service& ios, resolve_handler _handler) const
{
	resolver_ptr new_resolver = boost::make_shared<resolver>(boost::ref(ios));
	new_resolver->async_resolve(m_query, boost::bind(_handler, _1, _2, new_resolver));
}

}
}
