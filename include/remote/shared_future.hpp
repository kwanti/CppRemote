// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_SHARED_FUTURE_HPP__
#define __REMOTE_SHARED_FUTURE_HPP__

#include <remote/fwd.hpp>

#include <boost/move/move.hpp>
#include <boost/chrono.hpp>


namespace remote
{

template<typename T>
class shared_future
{
	typedef boost::shared_ptr<detail::basic_result<T> > impl_type;
	BOOST_COPYABLE_AND_MOVABLE(shared_future)

public:
	shared_future();
	shared_future(shared_future const& src);
	shared_future(BOOST_RV_REF(shared_future) src);
	shared_future(BOOST_RV_REF(future<T>) src);
	shared_future& operator = (BOOST_COPY_ASSIGN_REF(shared_future) src);
	shared_future& operator = (BOOST_RV_REF(shared_future) src);
	shared_future& operator = (BOOST_RV_REF(future<T>) src);

	void swap(shared_future& other);
	bool valid() const;

	T get();
	void wait() const;

	template<typename Rep, typename Period>
	future_status wait_for(boost::chrono::duration<Rep, Period> const& duration) const;

	template<typename Clock, typename Duration>
	future_status wait_until(boost::chrono::time_point<Clock, Duration> const& timeout_time) const;

private:
	impl_type m_impl;
};

template<typename T>
void swap(shared_future<T>& lhs, shared_future<T>& rhs);

}

#include <remote/shared_future.ipp>
#endif
