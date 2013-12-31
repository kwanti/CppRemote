// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_LOCAL_ACCEPTOR_HPP__
#define __REMOTE_BINDINGS_LOCAL_ACCEPTOR_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/local_endpoint.hpp>
#ifdef BOOST_ASIO_HAS_LOCAL_SOCKETS

#include <boost/asio/strand.hpp>
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class local_acceptor : public boost::noncopyable
{
public:
	typedef boost::function<void(exception_ptr)> handler;
	typedef local_endpoint endpoint_type;

public:
	explicit local_acceptor(io_service& ios);
	void listen(endpoint_type const& ep, handler const& hdl);
	void close();
	bool is_listening() const;

private:
	friend class local_transport;

	typedef local_acceptor this_type;
	typedef boost::asio::local::stream_protocol asio_local;

	typedef boost::system::error_code error_code;
	typedef boost::function<void(error_code)> accept_handler;

private:
	io_service& m_io_service;
	mutable io_service::strand m_strand;
	asio_local::acceptor m_acceptor;

	void do_listen(endpoint_type ep, handler hdl);

	void accept(asio_local::socket& socket, accept_handler const& hdl);
	void do_accept(asio_local::socket& socket, accept_handler hdl);

	void do_close();
};

}
}

#endif
#endif
