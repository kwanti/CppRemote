// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_BINDINGS_UDP_ENDPOINT_HPP__
#define REMOTE_BINDINGS_UDP_ENDPOINT_HPP__

#include <boost/asio/ip/udp.hpp>
#include <boost/function.hpp>
#include <string>


namespace remote
{
namespace bindings
{

class udp_endpoint
{
public:
	typedef boost::asio::ip::udp asio_udp;

public:
	udp_endpoint(std::string const& host, std::string const& svc);
	udp_endpoint(std::string const& host, int port);
	udp_endpoint(asio_udp const& version, std::string const& svc);
	udp_endpoint(asio_udp const& version, int port);
	explicit udp_endpoint(std::string const& svc);
	explicit udp_endpoint(int port);
	explicit udp_endpoint(asio_udp::endpoint const& ep);

	static asio_udp v4() { return asio_udp::v4(); }
	static asio_udp v6() { return asio_udp::v6(); }

private:
	friend class udp_acceptor;
	friend class udp_transport;
	typedef asio_udp::resolver resolver;
	typedef boost::asio::io_service io_service;
	typedef boost::system::error_code error_code;

	typedef boost::shared_ptr<resolver> resolver_ptr;
	typedef boost::function<void(error_code const&, resolver::iterator, resolver_ptr)> resolve_handler;

private:
	resolver::query m_query;
	asio_udp::endpoint m_endpoint;

	void resolve(io_service& ios, resolve_handler _handler) const;
};

}
}

#endif
