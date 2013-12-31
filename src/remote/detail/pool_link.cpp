// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/pool_link.hpp>
#include <remote/detail/pool_node.hpp>
#include <remote/detail/service_pool.hpp>
#include <remote/detail/proxy_pool.hpp>
#include <remote/detail/call.hpp>
#include <remote/detail/result.hpp>

#include <boost/uuid/nil_generator.hpp>
#include <boost/make_shared.hpp>


namespace remote
{
namespace detail
{

pool_link::pool_link(pool_node_ptr const& node, io_service& ios)
: m_weak_node(node)
, m_io_service(ios)
, m_pool_id(boost::uuids::nil_uuid())
, m_call_timeout(static_cast<boost::chrono::milliseconds::rep>(default_timeout))
{}

pool_link::~pool_link()
{}

void pool_link::initialize(pool_link_ptr const& self)
{
	BOOST_ASSERT(!m_proxy_pool);
	m_proxy_pool = boost::make_shared<proxy_pool>(self);

	pool_node_ptr node = m_weak_node.lock();
	BOOST_ASSERT(node);

	node->register_pool(m_proxy_pool);
}

void pool_link::shutdown()
{
	pool_node_ptr node = m_weak_node.lock();
	BOOST_ASSERT(node);

	node->unregister_pool(m_proxy_pool);
}

proxy_pool& pool_link::get_proxy_pool()
{
	BOOST_ASSERT(m_proxy_pool);
	return *m_proxy_pool;
}

pool_node_ptr pool_link::get_pool_node()
{
	return m_weak_node.lock();
}

bool pool_link::is_attached() const
{
	return (m_pool_id != boost::uuids::nil_uuid());
}

void pool_link::attach(boost::uuids::uuid const& pid)
{
	m_pool_id = pid;
	// TODO: update proxy pool here
}

void pool_link::detach()
{
	if(m_pool_id.is_nil())
		return;

	// cancel all pending results
	cancel_results(boost::make_shared<remote_error>
		(remote_error::operation_aborted, "link is disconnected"));

	m_pool_id = boost::uuids::nil_uuid();
}

void pool_link::cancel_results(exception_ptr e)
{
	boost::lock_guard<boost::mutex> lock(m_results_mutex);
	for(weak_results::iterator iter = m_results.begin(); iter != m_results.end(); ++iter)
	{
		result_ptr _result = iter->second.lock();
		if(_result)
			_result->cancel(e);
	}
	m_results.clear();
}

void pool_link::invoke(call_ptr const& _call, result_ptr const& _result)
{
	call_id cid = m_call_id.next();
	BOOST_ASSERT(cid);

	_result->set_id(cid);
	_result->set_io_service(&m_io_service);
	{
		boost::lock_guard<boost::mutex> lock(m_results_mutex);
		m_results[cid] = _result;
	}
	_result->set_timeout(m_call_timeout);

	_call->set_id(cid);
	send(_call);
}

void pool_link::invoke(call_ptr const& _call)
{
	send(_call);
}

void pool_link::dispatch(call_ptr _call)
{
	BOOST_ASSERT(_call);

	if(_call->ready())
	{
		result_ptr _result;
		{
			boost::lock_guard<boost::mutex> lock(m_results_mutex);
			weak_results::iterator iter = m_results.find(_call->id());
			if(iter == m_results.end())
				return;	/// TODO: log warning here

			_result = iter->second.lock();
			m_results.erase(iter);
		}

		if(!_result)
			return;	// nobody is holding the result. so, no need to update.

		_result->update(m_weak_node.lock(), _call);
	}
	else
	{
		service_ptr svc;

		pool_node_ptr node = m_weak_node.lock();
		if(node)
		{
			svc = node->get_service_pool().find(_call->object_id());
		}

		if(svc)
		{
			_call->invoke(node, svc);
		}
		else if(!_call->one_way())
		{
			_call->set_exception(boost::make_shared<remote_error>
								(remote_error::service_not_found,
								"service not found when dispatching call"));
		}

		if(_call->one_way())
			return;

		send(_call);
	}
}

}
}
