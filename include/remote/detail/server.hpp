// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_SERVER_HPP__
#define REMOTE_DETAIL_SERVER_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/session.hpp>

#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/container/set.hpp>
#include <boost/asio/strand.hpp>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace detail
{

class server
: public boost::noncopyable
, public boost::enable_shared_from_this<server>
{
public:
	enum state_t {stopped, starting, started, stopping};

	typedef boost::container::set<session> sessions;
	typedef boost::function<void(exception_ptr)> handler;
	typedef boost::function<void(exception_ptr, session&)> accept_handler;
	static void default_handler(exception_ptr) {}
	static void default_accept_handler(exception_ptr, session&) {}

public:
	server(pool_node_ptr const& node, io_service& ios);

	void shutdown();

	//sessions get_sessions() const { return m_sessions; }

	pool_node_ptr get_pool_node() const { return m_node; }
	void set_accept_handler(accept_handler const& _handler);

	void start(binding_ptr const& _binding, handler const& _handler);
	void stop(handler const& _handler);

	state_t state() const { return m_state; }
	bool ready() const { return ((m_state == stopped) || (m_state == started)); }
	state_t wait_for_ready() const;

private:
	class scoped_decrease: public boost::noncopyable
	{
	public:
		scoped_decrease(int& value) : m_value(value) {}
		~scoped_decrease() { --m_value; }
	private:
		int& m_value;
	};

private:
	io_service& m_io_service;
	mutable io_service::strand m_strand;
	state_t m_state;
	acceptor_ptr m_acceptor;
	sessions m_sessions;

	accept_handler m_accept_handler;
	int m_pending_io;

	pool_node_ptr m_node;

private:
	static session& invalid_session();
	static bool is_not_active_session(session& _session);
	void do_set_accept_handler(accept_handler _handler);
	void do_start(binding_ptr _binding, handler _handler);
	void on_listen(exception_ptr _exception, handler _handler);
	void do_accept();
	void on_accept(exception_ptr _exception, comm_link_ptr link);
	void do_stop(handler _handler);
	void signal_stop(handler _handler);
	void do_stop_session(session& _session);
	void on_session_stopped(exception_ptr);
};

}
}

#endif
