// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_UDP_ACCEPTOR_HPP__
#define __REMOTE_BINDINGS_UDP_ACCEPTOR_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/udp_endpoint.hpp>

#include <boost/asio/strand.hpp>
#include <boost/asio/ip/udp.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class udp_acceptor : public boost::noncopyable
{
public:
	typedef boost::function<void(exception_ptr)> handler;

public:
	explicit udp_acceptor(io_service& ios);
	void listen(udp_endpoint const& ep, handler const& hdl);
	void close();
	bool is_listening() const;

private:
	friend class udp_transport;

	typedef udp_acceptor this_type;
	typedef boost::asio::ip::udp asio_udp;
	typedef asio_udp::resolver::iterator iterator;
	typedef boost::shared_ptr<asio_udp::resolver> resolver_ptr;

	typedef boost::system::error_code error_code;
	typedef boost::function<void(error_code)> accept_handler;

private:
	io_service& m_io_service;
	mutable io_service::strand m_strand;
	asio_udp::socket m_acceptor;

private:
	void on_resolve(error_code ec, iterator iter, resolver_ptr resolver, handler hdl);
	void do_close();

	void accept(asio_udp::socket& socket, accept_handler const& hdl);
	void do_accept(asio_udp::socket& socket, accept_handler hdl);

// implementation
private:
	static const std::size_t mtu_size = 1500;

	struct accept_operation;
	typedef boost::shared_ptr<accept_operation> accept_operation_ptr;

private:
	void udp_async_accept(asio_udp::socket& socket, accept_handler const& hdl);
	void udp_retry_accept(accept_operation_ptr op);
	void udp_on_accept(error_code ec, std::size_t tx, accept_operation_ptr op);
};

}
}

#endif
