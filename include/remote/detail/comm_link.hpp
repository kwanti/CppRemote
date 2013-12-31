// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_COMM_LINK_HPP__
#define __REMOTE_DETAIL_COMM_LINK_HPP__

#include <remote/detail/pool_link.hpp>

#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/strand.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/function.hpp>

#include <map>


namespace remote
{
namespace detail
{

template<typename Target>
class comm_request_service;

class comm_link
: public pool_link
, public boost::enable_shared_from_this<comm_link>
{
public:
	enum state_t { stopped, starting, started, stopping };

	typedef boost::asio::io_service io_service;
	typedef boost::function<void(exception_ptr)> handler;
	static void default_handler(exception_ptr){}

public:
	comm_link(pool_node_ptr const& node, io_service& ios);

	void initialize();
	void shutdown();

	void set_error_handler(handler _handler);

	void accept(acceptor_ptr const& _acceptor, handler const& _handler);
	void connect(binding_ptr const& _binding, handler const& _handler);
	void disconnect(handler const& _handler);

	state_t state() const { return m_state; }
	bool ready() const { return ((m_state == stopped) || (m_state == started)); }

	void send(call_ptr const& _call);
	object_id resolve(std::string const& name, bool& cache);

private:
	class scoped_decrease
	{
	public:
		scoped_decrease(int& value) : m_value(value) {}
		~scoped_decrease() { --m_value; }
	private:
		int& m_value;

		scoped_decrease const& operator = (scoped_decrease const&);
	};

	class comm_request_handler
	{
	public:
		comm_request_handler(comm_link& link);
		boost::uuids::uuid initialize(boost::uuids::uuid pid);
		object_id resolve(std::string const& name);

	private:
		comm_link& m_link;

		comm_request_handler const& operator = (comm_request_handler const&);
	};

	typedef comm_request_service<comm_request_handler> comm_service_type;
	typedef std::map<std::string, object_id> named_proxies;
	static const object_vid m_comm_oid;

private:
	mutable io_service::strand m_strand;
	state_t m_state;
	channel_ptr m_channel;

	int m_pending_io;
	handler m_error_handler;

	mutable boost::mutex m_proxies_mutex;
	named_proxies m_named_proxies;

	boost::shared_ptr<comm_request_handler> m_comm_handler;
	service_ptr m_comm_request_service;

private:
	void do_set_error_handler(handler _handler);
	void do_accept(acceptor_ptr _acceptor, handler _handler);
	void do_start(binding_ptr _binding, handler _handler);
	void on_connect(exception_ptr _exception, handler _handler);
	void signal_stop(handler _handler);
	void do_stop(exception_ptr _exception, handler _handler);
	void do_send(call_ptr _call);
	void on_send(exception_ptr _exception, call_ptr _call);
	void on_receive(exception_ptr _exception, call_ptr _call);

	bool dispatch_comm_request(call_ptr const& _call);
	bool is_connection_error(exception_ptr const& _exception);

public:

#define REMOTE_CC BOOST_PP_EMPTY()
#include <remote/detail/comm_link_invoke.ipp>
#undef REMOTE_CC

#define REMOTE_CC const
#include <remote/detail/comm_link_invoke.ipp>
#undef REMOTE_CC

};

}
}

#endif // __REMOTE_DETAIL_COMM_LINK_HPP__
