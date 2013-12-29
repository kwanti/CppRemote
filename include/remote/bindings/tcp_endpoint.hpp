// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_BINDINGS_TCP_ENDPOINT_HPP__
#define REMOTE_BINDINGS_TCP_ENDPOINT_HPP__

#include <boost/asio/ip/tcp.hpp>
#include <boost/function.hpp>
#include <string>


namespace remote
{
namespace bindings
{

class tcp_endpoint
{
public:
	typedef boost::asio::ip::tcp asio_tcp;

public:
	tcp_endpoint(std::string const& host, std::string const& svc);
	tcp_endpoint(std::string const& host, int port);
	tcp_endpoint(asio_tcp const& version, std::string const& svc);
	tcp_endpoint(asio_tcp const& version, int port);
	explicit tcp_endpoint(std::string const& svc);
	explicit tcp_endpoint(int port);

	static asio_tcp v4() { return asio_tcp::v4(); }
	static asio_tcp v6() { return asio_tcp::v6(); }

private:
	friend class tcp_acceptor;
	friend class tcp_transport;
	typedef asio_tcp::resolver resolver;
	typedef boost::asio::io_service io_service;
	typedef boost::system::error_code error_code;

	typedef boost::shared_ptr<resolver> resolver_ptr;
	typedef boost::function<void(error_code const&, resolver::iterator, resolver_ptr)> resolve_handler;

private:
	resolver::query m_query;

	void resolve(io_service& ios, resolve_handler _handler) const;
};

}
}

#endif
