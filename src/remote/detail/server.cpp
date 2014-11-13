// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/detail/server.hpp>
#include <remote/bindings/binding.hpp>
#include <remote/bindings/acceptor.hpp>
#include <remote/transport_error.hpp>
#include <remote/remote_error.hpp>
#include <remote/io_runner.hpp>

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/assert.hpp>


namespace remote
{
namespace detail
{


template<typename Iter, typename Pred>
Iter remove_if_(Iter first, Iter last, Pred pred)
{
	first = std::find_if(first, last, pred);
	if(first == last)
		return first;

	Iter result = first;
	++first;
	for(; first != last; ++first)
	if(!pred(*first))
	{
		*result = boost::move(*first);
		++result;
	}
	return result;
}


server::server(pool_node_ptr const& node, io_service& ios)
: m_io_service(ios)
, m_strand(ios)
, m_state(stopped)
, m_accept_handler(&server::default_accept_handler)
, m_pending_io(0)
, m_node(node)
{}

void server::shutdown()
{
	if((m_state == stopped) || m_io_service.stopped())
		return;

	stop(default_handler);
	do
	{
		if(!io_runner::poll_one(m_io_service))
			boost::this_thread::yield();
	}
	while((m_state != stopped) && !m_io_service.stopped());
}

void server::set_accept_handler(accept_handler const& _handler)
{
	m_strand.dispatch(boost::bind(&server::do_set_accept_handler, shared_from_this(), _handler));
}

void server::do_set_accept_handler(accept_handler _handler)
{
	m_accept_handler = _handler;
}

void server::start(bindings::binding_ptr const& _binding, handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_strand.dispatch(boost::bind(&server::do_start, shared_from_this(), _binding, _handler));
}

void server::do_start(bindings::binding_ptr _binding, handler _handler)
{
	BOOST_ASSERT(_handler);
	if(m_state != stopped)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_pending, "other start or stop operation pending")));
		return;
	}

	if(!_binding)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_pending, "null end point as parameter")));
		return;
	}

	m_state = starting;
	++m_pending_io;
	m_acceptor = _binding->listen(m_io_service, m_strand.wrap(boost::bind
		(&server::on_listen, this, _1, _handler)));
}

void server::on_listen(exception_ptr _exception, handler _handler)
{
	BOOST_ASSERT(_handler);
	scoped_decrease sd(m_pending_io);

	if(m_state != starting)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_aborted, "start operation aborted")));
		return;
	}

	if(_exception)
	{
		signal_stop(&server::default_handler);
		m_io_service.post(boost::bind(_handler, _exception));
		return;
	}

	BOOST_ASSERT(m_acceptor);
	BOOST_ASSERT(m_acceptor->is_listening());

	m_state = started;
	m_io_service.post(boost::bind(_handler, _exception));

	do_accept();
}

void server::do_accept()
{
	comm_link_ptr link = boost::make_shared<comm_link>(m_node, boost::ref(m_io_service));
	link->initialize();

	++m_pending_io;
	link->accept(m_acceptor, m_strand.wrap(boost::bind(&server::on_accept, this, _1, link)));
}

void server::on_accept(exception_ptr _exception, comm_link_ptr link)
{
	BOOST_ASSERT(link);
	scoped_decrease sd(m_pending_io);

	if(_exception)
	{
		if(m_state != stopping)
		{
			m_io_service.post(boost::bind(m_accept_handler,
				_exception, boost::ref(invalid_session())));
		}

		link->shutdown();
		if(m_state == started)	// should we stop the server?
			signal_stop(&server::default_handler);

		return;
	}

	if(m_state == stopping)
	{
		link->shutdown();
		return;
	}

	BOOST_ASSERT(m_accept_handler);
	BOOST_ASSERT(m_state == started);

	// remove stopped and invalid sessions from server
	m_sessions.erase(remove_if_(m_sessions.begin(), m_sessions.end(),
		is_not_active_session), m_sessions.end());

	sessions::iterator iter = m_sessions.emplace(link).first;
	m_io_service.post(boost::bind(m_accept_handler, _exception, boost::ref(*iter)));

	// continue to accept next session
	do_accept();
}

void server::stop(handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_strand.dispatch(boost::bind(&server::signal_stop, shared_from_this(), _handler));
}

void server::signal_stop(handler _handler)
{
	BOOST_ASSERT(_handler);
	if(m_state == stopped)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::already_stopped, "server already stopped")));
		return;
	}

	if(m_state == stopping)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_pending, "another stop operation is pending")));
		return;
	}

	BOOST_ASSERT(m_acceptor);

	// signal stopping
	m_state = stopping;

	// stop all sessions
	std::for_each(m_sessions.begin(), m_sessions.end(),
		boost::bind(&server::do_stop_session, this, _1));

	// abort all io operations
	m_acceptor->close();

	++m_pending_io;
	m_io_service.post(m_strand.wrap(boost::bind(&server::do_stop, this, _handler)));
}

void server::do_stop(handler _handler)
{
	BOOST_ASSERT(_handler);
	BOOST_ASSERT(m_state == stopping);
	BOOST_ASSERT(m_pending_io > 0);

	if(m_pending_io > 1)
	{
		m_strand.post(boost::bind(&server::do_stop, this, _handler));
		return;
	}

	BOOST_ASSERT(m_acceptor);
	BOOST_ASSERT(!m_acceptor->is_listening());
	BOOST_ASSERT(m_pending_io == 1);

	m_sessions.clear();
	m_acceptor.reset();
	m_pending_io = 0;
	m_state = stopped;

	m_io_service.post(boost::bind(_handler, exception_ptr()));
}

server::state_t server::wait_for_ready() const
{
	for(int i=0; i<20; ++i)
	{
		if(!ready())
			break;
		boost::this_thread::sleep_for(boost::chrono::milliseconds(10));
	}

	while(!ready())
	{
		boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
	}

	return m_state;
}

void server::do_stop_session(session& _session)
{
	if(!_session.m_link)
		return;

	++m_pending_io;
	_session.stop(m_strand.wrap(boost::bind
		(&server::on_session_stopped, this, _1)));
}

void server::on_session_stopped(exception_ptr)
{
	scoped_decrease sd(m_pending_io);
}

session& server::invalid_session()
{
	static session::no_init tag;
	static session s(tag);
	return s;
}

bool server::is_not_active_session(session& _session)
{
	return ((!_session.m_link) || (_session.state() == session::stopped));
}

}
}
