// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_TYPE_ID_HPP__
#define __REMOTE_DETAIL_TYPE_ID_HPP__

#include <remote/detail/counter.hpp>

#include <boost/cstdint.hpp>
#include <boost/thread/once.hpp>


namespace remote
{
namespace detail
{

class type_id_base
{
public:
	typedef boost::uint16_t value_type;

protected:
	static counter<value_type> m_counter;

private:
	type_id_base() {}
	type_id_base(type_id_base const&);
	void operator = (type_id_base const&);
};

template<typename T>
class type_id : public type_id_base
{
public:
	static value_type get()
	{
		boost::call_once(m_once, initialize);

		BOOST_ASSERT(m_id);
		return m_id;
	}

private:
	static boost::once_flag m_once;
	static value_type m_id;

private:
	static void initialize()
	{
		m_id = m_counter.next();
	}
};

template<typename T>
boost::once_flag type_id<T>::m_once = BOOST_ONCE_INIT;

template<typename T>
type_id_base::value_type type_id<T>::m_id = 0;

}
}

#endif
