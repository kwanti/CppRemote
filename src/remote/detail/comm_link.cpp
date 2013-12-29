// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/comm_link.hpp>
#include <remote/detail/comm_request.hpp>
#include <remote/detail/pool_node.hpp>
#include <remote/detail/service_pool.hpp>
#include <remote/remote_error.hpp>
#include <remote/io_runner.hpp>

#include <remote/bindings/binding.hpp>
#include <remote/bindings/acceptor.hpp>
#include <remote/bindings/channel.hpp>
#include <remote/system_error.hpp>

#include <boost/bind/bind.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4355)	// 'this' : used in base member initializer list
#endif


namespace remote
{
namespace detail
{

const object_vid comm_link::m_comm_oid(0);

comm_link::comm_link(pool_node_ptr const& node, io_service& ios)
: pool_link(node, ios)
, m_strand(ios)
, m_state(stopped)
, m_pending_io(0)
, m_error_handler(&default_handler)
, m_comm_handler(boost::make_shared<comm_request_handler>(boost::ref(*this)))
, m_comm_request_service(boost::make_shared<comm_service_type>(m_comm_handler, 0))
{}

void comm_link::initialize()
{
	pool_link::initialize(shared_from_this());
}

void comm_link::shutdown()
{
	if((state() == stopped) || m_io_service.stopped())
		return;

	disconnect(default_handler);
	do
	{
		if(!io_runner::poll_one(m_io_service))
			boost::this_thread::yield();
	}
	while((state() != stopped) && !m_io_service.stopped());

	pool_link::shutdown();
}

void comm_link::set_error_handler(handler _handler)
{
	m_strand.dispatch(boost::bind(&comm_link::do_set_error_handler, shared_from_this(), _handler));
}

void comm_link::do_set_error_handler(handler _handler)
{
	m_error_handler = _handler;
}

void comm_link::accept(acceptor_ptr const& _acceptor, handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_strand.dispatch(boost::bind(&comm_link::do_accept, shared_from_this(), _acceptor, _handler));
}

void comm_link::do_accept(acceptor_ptr _acceptor, handler _handler)
{
	BOOST_ASSERT(_handler);
	if(m_state != stopped)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_pending, "another start or stop operation is pending")));
		return;
	}

	if(!_acceptor)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::invalid_endpoint, "null acceptor as parameter")));
		return;
	}

	m_state = starting;
	++m_pending_io;
	m_channel = _acceptor->accept(m_strand.wrap
		(boost::bind(&comm_link::on_connect, this, _1, _handler)));
}

void comm_link::connect(binding_ptr const& _binding, handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_strand.dispatch(boost::bind(&comm_link::do_start, shared_from_this(), _binding, _handler));
}

void comm_link::do_start(binding_ptr _binding, handler _handler)
{
	BOOST_ASSERT(_handler);
	if(m_state != stopped)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_pending, "another start or stop operation is pending")));
		return;
	}

	if(!_binding)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::invalid_endpoint, "null end point as parameter")));
		return;
	}

	m_state = starting;
	++m_pending_io;
	m_channel = _binding->connect(m_io_service,
		m_strand.wrap(boost::bind(&comm_link::on_connect, this, _1, _handler)));
}

void comm_link::on_connect(exception_ptr _exception, handler _handler)
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
		signal_stop(default_handler);
		m_io_service.post(boost::bind(_handler, _exception));
		return;
	}

	BOOST_ASSERT(m_channel);
	BOOST_ASSERT(m_channel->state() == bindings::channel::started);

	++m_pending_io;
	m_channel->receive(m_strand.wrap(boost::bind(&comm_link::on_receive, this, _1, _2)));

	m_state = started;
	m_io_service.post(boost::bind(_handler, _exception));
}

void comm_link::disconnect(handler const& _handler)
{
	BOOST_ASSERT(_handler);
	m_strand.dispatch(boost::bind(&comm_link::signal_stop, shared_from_this(), _handler));
}

void comm_link::signal_stop(handler _handler)
{
	BOOST_ASSERT(_handler);
	if(m_state == stopped)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::already_stopped, "session already stopped")));
		return;
	}

	if(m_state == stopping)
	{
		m_io_service.post(boost::bind(_handler, boost::make_shared<remote_error>
			(remote_error::operation_pending, "another stop operation is pending")));
		return;
	}

	BOOST_ASSERT(m_channel);

	// signal stopping
	m_state = stopping;

	// detach link
	detach();

	++m_pending_io;
	m_channel->disconnect(m_strand.wrap(boost::bind(&comm_link::do_stop, this, _1, _handler)));
}

void comm_link::do_stop(exception_ptr _exception, handler _handler)
{
	BOOST_ASSERT(_handler);
	BOOST_ASSERT(m_state == stopping);
	BOOST_ASSERT(m_pending_io > 0);

	if(m_pending_io > 1)
	{
		// if this is not the last io handler, post to strand again
		// to wait for other io operation to return
		m_strand.post(boost::bind(&comm_link::do_stop, this, _exception, _handler));
		return;
	}

	BOOST_ASSERT(m_pending_io == 1);
	BOOST_ASSERT(m_channel);
	BOOST_ASSERT(m_channel->state() == bindings::channel::stopped);

	// must be in strand
	m_channel.reset();
	m_pending_io = 0;
	m_state = stopped;

	m_io_service.post(boost::bind(_handler, _exception));
}

void comm_link::send(call_ptr const& _call)
{
	BOOST_ASSERT(!m_io_service.stopped());
	m_strand.post(boost::bind(&comm_link::do_send, shared_from_this(), _call));
}

void comm_link::do_send(call_ptr _call)
{
	BOOST_ASSERT(_call);
	if(m_state != started)
	{
		_call->set_exception(boost::make_shared<remote_error>
			(remote_error::operation_aborted, "link is disconnected"));

		if(dispatch_comm_request(_call))
			return;

		m_io_service.post(boost::bind(&comm_link::dispatch, shared_from_this(), _call));
		return;
	}

	BOOST_ASSERT(m_channel);
	BOOST_ASSERT(m_channel->state() == bindings::channel::started);

	++m_pending_io;
	m_channel->send(_call, m_strand.wrap(boost::bind(&comm_link::on_send, this, _1, _call)));
}

void comm_link::on_send(exception_ptr _exception, call_ptr _call)
{
	scoped_decrease sd(m_pending_io);
	if(_exception)
	{
		if(is_connection_error(_exception))
		{
			if(m_state == started)
				signal_stop(default_handler);	// stop on connection error
		}

		if(m_state == started)
		{
			_call->set_exception(_exception);

			if(dispatch_comm_request(_call))
				return;

			// dispatch is thread safe. so, not posting to strand
			// execute call dispatch outside of strand so that io operations is not blocked
			m_io_service.post(boost::bind(&comm_link::dispatch, shared_from_this(), _call));
		}
	}
}

void comm_link::on_receive(exception_ptr _exception, call_ptr _call)
{
	if(_exception)
	{
		BOOST_ASSERT(m_error_handler);
		if(m_state != stopping)
			m_io_service.post(boost::bind(m_error_handler, _exception));

		if(is_connection_error(_exception))
		{
			// for receive handler. we only decrease pending io count on transport error
			// because receive operation will only stop when transport error occurred
			// or channel disconnected.
			scoped_decrease sd(m_pending_io);
			if(m_state == started)
				signal_stop(default_handler);	// stop on transport error
		}
		return;
	}

	BOOST_ASSERT(_call);

	if(dispatch_comm_request(_call))
		return;

	// dispatch is thread safe. so, not posting to strand
	// execute call dispatch outside of strand so that io operations is not blocked
	m_io_service.post(boost::bind(&comm_link::dispatch, shared_from_this(), _call));

}

object_id comm_link::resolve(std::string const& name, bool& cache)
{
	if(cache)
	{
		// try to get id from cached
		boost::lock_guard<boost::mutex> lock(m_proxies_mutex);
		named_proxies::iterator iter = m_named_proxies.find(name);
		if(iter != m_named_proxies.end())
			return iter->second;
	}

	cache = false;

	comm_request request(shared_from_this(), m_comm_oid);

	if(!is_attached())
	{
		pool_node_ptr node = m_weak_node.lock();
		BOOST_ASSERT(node);

		attach(request.initialize(node->get_service_pool().id()));
	}

	object_id id = request.resolve(name);
	if(id != 0)
	{
		// put the newly resolved id to cache
		boost::lock_guard<boost::mutex> lock(m_proxies_mutex);
		m_named_proxies[name] = id;
	}

	return id;
}

bool comm_link::dispatch_comm_request(call_ptr const& _call)
{
	BOOST_ASSERT(_call);
	BOOST_ASSERT(m_comm_request_service);

	if(!_call->ready() && (_call->object_id() == m_comm_oid))
	{
		pool_node_ptr node = m_weak_node.lock();
		BOOST_ASSERT(node);

		_call->invoke(node, m_comm_request_service);

		BOOST_ASSERT(!_call->one_way());	// not using one_way_call internally
		send(_call);
		return true;
	}
	return false;
}

bool comm_link::is_connection_error(exception_ptr const& _exception)
{
	try
	{
		rethrow_exception(_exception);
		BOOST_ASSERT(false);
	}
	catch(system_error& e)
	{
		if(e.code() == boost::system::errc::message_size)
			return false;

		return true;
	}
	catch(...)
	{}
	return false;
}

comm_link::comm_request_handler::comm_request_handler(comm_link& link)
: m_link(link)
{}

boost::uuids::uuid comm_link::comm_request_handler::initialize(boost::uuids::uuid pid)
{
	pool_node_ptr node = m_link.m_weak_node.lock();
	BOOST_ASSERT(node);

	m_link.attach(pid);
	return node->get_service_pool().id();
}

object_id comm_link::comm_request_handler::resolve(std::string const& name)
{
	pool_node_ptr node = m_link.m_weak_node.lock();
	BOOST_ASSERT(node);

	return node->get_service_pool().resolve(name);
}

}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
