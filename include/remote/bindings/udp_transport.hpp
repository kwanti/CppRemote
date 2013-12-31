// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_UDP_TRANSPORT_HPP__
#define __REMOTE_BINDINGS_UDP_TRANSPORT_HPP__

#include <remote/bindings/fwd.hpp>
#include <remote/bindings/buffer.hpp>
#include <remote/bindings/udp_endpoint.hpp>
#include <remote/bindings/udp_acceptor.hpp>

#include <boost/asio/strand.hpp>
#include <boost/asio/basic_waitable_timer.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class udp_transport : public boost::noncopyable
{
public:
	enum state_t {stopped, starting, started, stopping};

	typedef udp_acceptor acceptor_type;
	typedef udp_endpoint endpoint_type;

	typedef boost::function<void(exception_ptr)> handler;
	typedef boost::function<void(exception_ptr, buffer_ptr)> read_handler;

	static void default_handler(exception_ptr){}

public:
	explicit udp_transport(io_service& ios);
	~udp_transport();

	state_t state() const { return m_state; }

	endpoint_type local_endpoint() const;
	endpoint_type remote_endpoint() const;

	void accept(acceptor_type& ac, handler const& hdl);
	void connect(endpoint_type const& ep, handler const& hdl);
	void disconnect(handler const& hdl);

	void write(buffer_ptr const& buf, handler const& hdl);
	void read(read_handler const& hdl);

private:
	typedef udp_transport this_type;

	typedef boost::system::error_code error_code;
	typedef boost::asio::ip::udp asio_udp;
	typedef asio_udp::resolver::iterator iterator;
	typedef boost::shared_ptr<asio_udp::resolver> resolver_ptr;
	typedef boost::function<void(error_code)> impl_handler;

	static const error_code more_data_available;
	static const std::size_t mtu_size = 1500;					// assumed mtu size
	static const std::size_t max_write_size = mtu_size - 300;	// assumed max write size
	static const boost::chrono::milliseconds::rep default_connect_timeout = 3000;	// milliseconds

	struct connect_operation;
	typedef boost::shared_ptr<connect_operation> connect_operation_ptr;

	struct write_operation;
	typedef boost::shared_ptr<write_operation> write_operation_ptr;

private:
	state_t m_state;
	io_service& m_io_service;
	mutable io_service::strand m_strand;
	asio_udp::socket m_socket;

	boost::asio::basic_waitable_timer<boost::chrono::system_clock> m_timer;
	boost::chrono::milliseconds m_connect_timeout;

	buffer_ptr m_read_buffer;
	read_handler m_read_handler;

private:
	void do_accept(acceptor_type& ac, handler hdl);
	void on_accept(error_code ec, handler hdl);

	void on_resolve(error_code ec, iterator iter, resolver_ptr rsv, handler hdl);

	void do_connect(endpoint_type ep, handler hdl);
	void on_connect(error_code ec, iterator iter, handler hdl);

	void udp_connect(asio_udp::endpoint ep, impl_handler hdl);
	void on_udp_connect(error_code ec, std::size_t tx, connect_operation_ptr op);
	void on_udp_connect_timeout(error_code ec, impl_handler hdl);


	void signal_disconnect(handler hdl);
	void do_disconnect(error_code ec, handler hdl);

	void start_read(read_handler hdl);
	void do_read();
	void on_read(error_code ec, std::size_t tx);

	void do_write(buffer_ptr buf, handler hdl);
	void on_write(error_code ec, std::size_t tx, write_operation_ptr op);
};

}
}

#endif
