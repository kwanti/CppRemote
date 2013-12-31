// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_NAMED_PIPE_ACCEPTOR_HPP__
#define __REMOTE_BINDINGS_NAMED_PIPE_ACCEPTOR_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/named_pipe_endpoint.hpp>
#ifdef BOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE

#include <boost/asio/strand.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class named_pipe_acceptor : public boost::noncopyable
{
public:
	typedef boost::function<void(exception_ptr)> handler;
	typedef named_pipe_endpoint endpoint_type;

public:
	explicit named_pipe_acceptor(io_service& ios);
	void listen(endpoint_type const& ep, handler const& hdl);
	void close();
	bool is_listening() const;

private:
	friend class named_pipe_transport;

	typedef named_pipe_acceptor this_type;
	typedef boost::asio::windows::stream_handle stream_handle;

	typedef boost::system::error_code error_code;
	typedef boost::function<void(error_code)> accept_handler;

private:
	io_service& m_io_service;
	mutable io_service::strand m_strand;
	HANDLE m_acceptor;

private:
	void do_listen(endpoint_type ep, handler hdl);

	void accept(stream_handle& pipe, accept_handler const& hdl);
	void do_accept(stream_handle& pipe, accept_handler hdl);

// implementation
private:
	struct accept_operation;
	typedef boost::shared_ptr<accept_operation> accept_operation_ptr;

	endpoint_type m_endpoint;
	int m_pending;

	void pipe_listen(endpoint_type const& ep, error_code& ec);
	void pipe_async_accept(stream_handle& pipe, accept_handler const& hdl);
	void pipe_poll_accept(accept_operation_ptr op);

	void do_close();
	void poll_close();
	void poll_io_service() const;
};

}
}

#endif
#endif
