// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/result.hpp>
#include <remote/detail/utility.hpp>
#include <remote/io_runner.hpp>

#include <boost/smart_ptr/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace detail
{

result::result()
: m_id(static_cast<call_id>(-1))
, m_ready(false)
, m_timeout_time(time_point::max())
, m_io_service(0)
{}

result::result(one_way const&)
: m_id(0)
, m_ready(true)
, m_exception(make_no_result_error())
, m_io_service(0)
{}

result::result(exception_ptr const& e)
: m_id(static_cast<call_id>(-1))
, m_ready(true)
, m_exception(e)
, m_io_service(0)
{}

void result::update(pool_node_ptr const& node, call_ptr const& _call)
{
	{
		boost::lock_guard<boost::mutex> lock(m_mutex);
		if(m_ready)
			return;

		BOOST_ASSERT(m_id);
		BOOST_ASSERT(!m_exception);
		BOOST_ASSERT(m_pointers.empty());

		if(_call && (_call->id() == m_id))
		{
			boost::swap(m_exception, _call->m_exception);
			if(!m_exception)
			{
				try
				{
					boost::swap(m_pointers, _call->m_pointers);
					update_param(*node, _call);
				}
				catch(...)
				{
					m_exception = current_exception();
				}
			}
		}
		else
		{
			m_exception = boost::make_shared<remote_error>
				(remote_error::invalid_return_type, "invalid return call");
		}
		m_ready = true;
	}
	m_condition.notify_all();
}

void result::cancel(exception_ptr const& e)
{
	{
		boost::lock_guard<boost::mutex> lock(m_mutex);
		if(m_ready)
			return;

		m_exception = e;
		m_ready = true;
	}
	m_condition.notify_all();
}

void result::wait() const
{
	boost::unique_lock<boost::mutex> lock(m_mutex);
	wait(lock);
}

void result::wait(boost::unique_lock<boost::mutex>& lock) const
{
	BOOST_VERIFY(wait_until(time_point::max(), lock) == future_status::ready);
}

future_status result::wait_until(time_point const& wait_timeout_time,
						boost::unique_lock<boost::mutex>& lock) const
{
	if(ready(lock))
		return future_status::ready;

	while(!expired() && !(clock_type::now() > wait_timeout_time))
	{
		// note: don't set to wait for 0 ms, it might not timeout
		if(m_condition.wait_for(lock, boost::chrono::milliseconds(1),
			boost::bind(&result::ready, this, boost::ref(lock))))
		{
			return future_status::ready;
		}

		if(expired() || (clock_type::now() > wait_timeout_time))
			break;

		lock.unlock();
		io_runner::poll_one(*m_io_service);
		lock.lock();
	}

	if(clock_type::now() > wait_timeout_time)
		return future_status::timeout;

	BOOST_ASSERT(expired());

	m_exception = boost::make_shared<remote_error>
		(remote_error::timeout, "remote call timeout");

	m_ready = true;
	return future_status::ready;
}

bool result::expired() const
{
	return clock_type::now() > m_timeout_time;
}

bool result::ready() const
{
	{
		boost::lock_guard<boost::mutex> lock(m_mutex);
		if(m_ready)
			return true;

		if(!expired())
			return false;

		m_exception = boost::make_shared<remote_error>
			(remote_error::timeout, "remote call timeout");

		m_ready = true;
	}
	m_condition.notify_all();
	return true;
}

void result::get(param<void>&)
{
	if(never_update())
		return;

	boost::unique_lock<boost::mutex> lock(m_mutex);
	wait(lock);

	BOOST_ASSERT(m_ready);
	if(m_exception)
		rethrow_exception(m_exception);
}

bool result::ready(boost::unique_lock<boost::mutex>&) const
{
	if(!m_ready && expired())
	{
		m_exception = boost::make_shared<remote_error>
			(remote_error::timeout, "remote call timeout");

		m_ready = true;
	}
	return m_ready;
}

exception_ptr result::make_no_result_error()
{
	static remote_error e(remote_error::no_result, "no result for one way call");
	return exception_ptr(&e, null_deleter());
}

}
}
