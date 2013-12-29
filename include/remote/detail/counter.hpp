// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_COUNTER_HPP__
#define __REMOTE_DETAIL_COUNTER_HPP__

#include <boost/type_traits/is_integral.hpp>
#include <boost/static_assert.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>


namespace remote
{
namespace detail
{

template<typename T>
class counter
{
	BOOST_STATIC_ASSERT(boost::is_integral<T>::value);

	T m_id;
	boost::mutex m_mutex;

public:
	counter()
	: m_id(0)
	{}

	counter(T t)
	: m_id(t)
	{}

	inline T next()
	{
		boost::lock_guard<boost::mutex> lock(m_mutex);
		return ++m_id? m_id: ++m_id;
	}
};

}
}

#endif
