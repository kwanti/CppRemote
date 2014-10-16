// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_SHARED_FUTURE_IPP__
#define __REMOTE_SHARED_FUTURE_IPP__

#include <remote/shared_future.hpp>
#include <remote/future_error.hpp>
#include <remote/detail/result.hpp>

#include <boost/chrono.hpp>


namespace remote
{

template<typename T>
shared_future<T>::shared_future()
{}

template<typename T>
shared_future<T>::shared_future(shared_future const& src)
: m_impl(src.m_impl)
{}

template<typename T>
shared_future<T>::shared_future(BOOST_RV_REF(shared_future) src)
{
	std::swap(m_impl, src.m_impl);
	BOOST_ASSERT(!src.m_impl);
}

template<typename T>
shared_future<T>::shared_future(BOOST_RV_REF(future<T>) src)
{
	std::swap(m_impl, src.m_impl);
	BOOST_ASSERT(!src.m_impl);
}

template<typename T>
shared_future<T>& shared_future<T>::operator = (BOOST_COPY_ASSIGN_REF(shared_future) src)
{
	if(this != &src)
	{
		m_impl = src.m_impl;
	}
	return *this;
}

template<typename T>
shared_future<T>& shared_future<T>::operator = (BOOST_RV_REF(shared_future) src)
{
	if(this != &src)
	{
		std::swap(m_impl, src.m_impl);
		src.m_impl.reset();
	}
	return *this;
}

template<typename T>
shared_future<T>& shared_future<T>::operator = (BOOST_RV_REF(future<T>) src)
{
	std::swap(m_impl, src.m_impl);
	src.m_impl.reset();
	return *this;
}

template<typename T>
void shared_future<T>::swap(shared_future& other)
{
	std::swap(m_impl, other.m_impl);
}

template<typename T>
bool shared_future<T>::valid() const
{
	return static_cast<bool>(m_impl);
}

template<typename T>
T shared_future<T>::get()
{
	if(!valid())
		throw future_error(future_errc::no_state);

	return m_impl->get();
}

template<typename T>
void shared_future<T>::wait() const
{
	if(!valid())
		throw future_error(future_errc::no_state);

	return m_impl->wait();
}

template<typename T>
template<typename Rep, typename Period>
future_status shared_future<T>::wait_for(boost::chrono::duration<Rep, Period> const& duration) const
{
	if(!valid())
		throw future_error(future_errc::no_state);

	return m_impl->wait_for(duration);
}

template<typename T>
template<typename Clock, typename Duration>
future_status shared_future<T>::wait_until(boost::chrono::time_point<Clock, Duration> const& timeout_time) const
{
	if(!valid())
		throw future_error(future_errc::no_state);

	return m_impl->wait_until(timeout_time);
}

template<typename T>
void swap(shared_future<T>& lhs, shared_future<T>& rhs)
{
	lhs.swap(rhs);
}

}

#endif
