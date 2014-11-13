// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_RESULT_HPP__
#define __REMOTE_DETAIL_RESULT_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/out_param.hpp>
#include <remote/detail/param.hpp>
#include <remote/detail/call.hpp>
#include <remote/remote_error.hpp>

#include <boost/asio/io_service.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/bind/bind.hpp>
#include <boost/ref.hpp>


namespace remote
{
namespace detail
{

class result
{
public:
	result();
	result(one_way const&);
	result(exception_ptr const& e);
	virtual ~result(){}

	void set_io_service(boost::asio::io_service* ios) { m_io_service = ios; }
	call_id id() const { return m_id; }
	void set_id(call_id cid) { m_id = cid; }

	void update(pool_node_ptr const& node, call_ptr const& _call);
	void cancel(exception_ptr const& e);

	bool expired() const;
	bool ready() const;

	void wait() const;

	template<typename Rep, typename Period>
	future_status wait_for(boost::chrono::duration<Rep, Period> const& wait_timeout) const
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		return wait_until(clock_type::now() + wait_timeout, lock);
	}

	template<typename Clock, typename Duration>
	future_status wait_until(boost::chrono::time_point<Clock, Duration> const& timeout_time) const
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);
		return wait_until(clock_type::now() + (timeout_time - Clock::now()), lock);
	}

	template<typename Rep, typename Period>
	void set_timeout(boost::chrono::duration<Rep, Period> const& duration)
	{
		BOOST_ASSERT(!expired());
		m_timeout_time = clock_type::now() + duration;
	}

protected:
	typedef call::container_type container_type;
	typedef boost::chrono::system_clock clock_type;
	typedef boost::chrono::time_point<clock_type> time_point;

protected:
	call_id m_id;
	mutable bool m_ready;
	time_point m_timeout_time;

	mutable boost::mutex m_mutex;
	mutable boost::condition m_condition;

	mutable exception_ptr m_exception;
	container_type m_pointers;

	boost::asio::io_service* m_io_service;

protected:
	virtual void update_param(pool_node&, call_ptr const&) {}

	template<typename R>
	R get(param<R>& r)
	{
		boost::unique_lock<boost::mutex> lock(m_mutex);

#ifdef _DEBUG
		if(never_update())
		{
			BOOST_ASSERT(m_ready);
			BOOST_ASSERT(m_exception);
		}
#endif

		wait(lock);

		BOOST_ASSERT(m_ready);
		if(m_exception)
			rethrow_exception(m_exception);

		return r.get();
	}

	void get(param<void>&);

	bool never_update() const
	{
		return (m_id == 0);
	}

private:
	bool ready(boost::unique_lock<boost::mutex>& lock) const;
	void wait(boost::unique_lock<boost::mutex>& lock) const;
	future_status wait_until(time_point const& wait_timeout_time,
							boost::unique_lock<boost::mutex>& lock) const;

	static exception_ptr make_no_result_error();

};

template<typename R>
class basic_result : public result
{
public:
	basic_result()
	{}

	basic_result(one_way const& mode)
	: result(mode)
	{}

	basic_result(exception_ptr const& e)
	: result(e)
	{}

	R get()
	{
		return result::get(m_r);
	}

protected:
	param<R> m_r;

	void update_return(pool_node& node, param<R>& r)
	{
		update_return(node, r, need_to_marshal<R>());
	}

private:
	void update_return(pool_node& node, param<R>& r, boost::mpl::true_)
	{
		m_r = marshal(node, r.get());
	}

	void update_return(pool_node&, param<R>& r, boost::mpl::false_)
	{
		m_r = r;
	}
};

template<typename R>
class result_0 : public basic_result<R>
{
public:
	typedef basic_call_0<R> call_type;

public:
	result_0(boost::shared_ptr<call_type> const&)
	{}

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		this->update_return(node, temp->m_r);
	}
};

template<typename R, typename A1>
class result_1 : public basic_result<R>
{
public:
	typedef basic_call_1<R, A1> call_type;

public:
	result_1(boost::shared_ptr<call_type> const& _call)
	: m_a1(_call->m_a1)
	{}

private:
	out_param<A1> m_a1;

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		m_a1.update(node, temp->m_a1);
		this->update_return(node, temp->m_r);
	}
};

template<typename R, typename A1, typename A2>
class result_2 : public basic_result<R>
{
public:
	typedef basic_call_2<R, A1, A2> call_type;

public:
	result_2(boost::shared_ptr<call_type> const& _call)
	: m_a1(_call->m_a1)
	, m_a2(_call->m_a2)
	{}

private:
	out_param<A1> m_a1;
	out_param<A2> m_a2;

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		m_a1.update(node, temp->m_a1);
		m_a2.update(node, temp->m_a2);
		this->update_return(node, temp->m_r);
	}
};

template<typename R, typename A1, typename A2, typename A3>
class result_3 : public basic_result<R>
{
public:
	typedef basic_call_3<R, A1, A2, A3> call_type;

public:
	result_3(boost::shared_ptr<call_type> const& _call)
	: m_a1(_call->m_a1)
	, m_a2(_call->m_a2)
	, m_a3(_call->m_a3)
	{}

private:
	out_param<A1> m_a1;
	out_param<A2> m_a2;
	out_param<A3> m_a3;

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		m_a1.update(node, temp->m_a1);
		m_a2.update(node, temp->m_a2);
		m_a3.update(node, temp->m_a3);
		this->update_return(node, temp->m_r);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4>
class result_4 : public basic_result<R>
{
public:
	typedef basic_call_4<R, A1, A2, A3, A4> call_type;

public:
	result_4(boost::shared_ptr<call_type> const& _call)
	: m_a1(_call->m_a1)
	, m_a2(_call->m_a2)
	, m_a3(_call->m_a3)
	, m_a4(_call->m_a4)
	{}

private:
	out_param<A1> m_a1;
	out_param<A2> m_a2;
	out_param<A3> m_a3;
	out_param<A4> m_a4;

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		m_a1.update(node, temp->m_a1);
		m_a2.update(node, temp->m_a2);
		m_a3.update(node, temp->m_a3);
		m_a4.update(node, temp->m_a4);
		this->update_return(node, temp->m_r);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5>
class result_5 : public basic_result<R>
{
public:
	typedef basic_call_5<R, A1, A2, A3, A4, A5> call_type;

public:
	result_5(boost::shared_ptr<call_type> const& _call)
	: m_a1(_call->m_a1)
	, m_a2(_call->m_a2)
	, m_a3(_call->m_a3)
	, m_a4(_call->m_a4)
	, m_a5(_call->m_a5)
	{}

private:
	out_param<A1> m_a1;
	out_param<A2> m_a2;
	out_param<A3> m_a3;
	out_param<A4> m_a4;
	out_param<A5> m_a5;

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		m_a1.update(node, temp->m_a1);
		m_a2.update(node, temp->m_a2);
		m_a3.update(node, temp->m_a3);
		m_a4.update(node, temp->m_a4);
		m_a5.update(node, temp->m_a5);
		this->update_return(node, temp->m_r);
	}
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
class result_6 : public basic_result<R>
{
public:
	typedef basic_call_6<R, A1, A2, A3, A4, A5, A6> call_type;

public:
	result_6(boost::shared_ptr<call_type> const& _call)
	: m_a1(_call->m_a1)
	, m_a2(_call->m_a2)
	, m_a3(_call->m_a3)
	, m_a4(_call->m_a4)
	, m_a5(_call->m_a5)
	, m_a6(_call->m_a6)
	{}

private:
	out_param<A1> m_a1;
	out_param<A2> m_a2;
	out_param<A3> m_a3;
	out_param<A4> m_a4;
	out_param<A5> m_a5;
	out_param<A6> m_a6;

private:
	void update_param(pool_node& node, call_ptr const& _call)
	{
		boost::shared_ptr<call_type> temp = boost::dynamic_pointer_cast<call_type>(_call);
		if(!temp)
			throw remote_error(remote_error::invalid_return_type, "invalid return call");

		m_a1.update(node, temp->m_a1);
		m_a2.update(node, temp->m_a2);
		m_a3.update(node, temp->m_a3);
		m_a4.update(node, temp->m_a4);
		m_a5.update(node, temp->m_a5);
		m_a6.update(node, temp->m_a6);
		this->update_return(node, temp->m_r);
	}
};

}
}

#endif
