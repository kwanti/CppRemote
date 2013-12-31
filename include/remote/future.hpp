// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_FUTURE_HPP__
#define __REMOTE_FUTURE_HPP__

#include <remote/shared_future.hpp>


namespace remote
{

template<typename T>
class future
{
	friend class shared_future<T>;
	typedef boost::shared_ptr<detail::basic_result<T> > impl_type;
	BOOST_MOVABLE_BUT_NOT_COPYABLE(future)

public:
	future();
	explicit future(impl_type const& impl);
	future(BOOST_RV_REF(future) src);
	future& operator = (BOOST_RV_REF(future) src);

	shared_future<T> share();
	bool valid() const;

	T get();
	void wait() const;

	template<typename Rep, typename Period>
	future_status wait_for(boost::chrono::duration<Rep, Period> const& duration) const;

	template<typename Clock, typename Duration>
	future_status wait_until(boost::chrono::time_point<Clock, Duration> const& timeout_time);

private:
	impl_type m_impl;
};

}

#include <remote/future.ipp>
#endif
