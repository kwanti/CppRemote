// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_TCP_ACCEPTOR_HPP__
#define __REMOTE_BINDINGS_TCP_ACCEPTOR_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/tcp_endpoint.hpp>

#include <boost/asio/strand.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class tcp_acceptor : public boost::noncopyable
{
public:
	typedef boost::function<void(exception_ptr)> handler;
	typedef tcp_endpoint endpoint_type;

public:
	explicit tcp_acceptor(io_service& ios);
	void listen(endpoint_type const& ep, handler const& hdl);
	void close();
	bool is_listening() const;

private:
	friend class tcp_transport;

	typedef tcp_acceptor this_type;
	typedef boost::asio::ip::tcp asio_tcp;
	typedef asio_tcp::resolver::iterator iterator;
	typedef boost::shared_ptr<asio_tcp::resolver> resolver_ptr;

	typedef boost::system::error_code error_code;
	typedef boost::function<void(error_code)> accept_handler;

private:
	io_service& m_io_service;
	mutable io_service::strand m_strand;
	asio_tcp::acceptor m_acceptor;

private:
	void on_resolve(error_code ec, iterator iter, resolver_ptr resolver, handler hdl);
	void do_close();

	void accept(asio_tcp::socket& socket, accept_handler const& hdl);
	void do_accept(asio_tcp::socket& socket, accept_handler hdl);

};

}
}

#endif
