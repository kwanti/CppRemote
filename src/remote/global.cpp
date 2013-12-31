// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/global.hpp>

#include <boost/bind/bind.hpp>


namespace remote
{

remote::io_runner* global::m_io_runner = 0;
bindings::buffer_pool* global::m_buffer_pool = 0;

boost::once_flag global::m_io_runner_once = BOOST_ONCE_INIT;
boost::once_flag global::m_buffer_pool_once = BOOST_ONCE_INIT;

remote::io_runner& global::io_runner(size_t concurrency)
{
	boost::call_once(m_io_runner_once, boost::bind(make_io_runner, concurrency));

	BOOST_ASSERT(m_io_runner);
	return *m_io_runner;
}

bindings::buffer_pool& global::buffer_pool(size_t size)
{
	boost::call_once(m_buffer_pool_once, boost::bind(make_buffer_pool, size));

	BOOST_ASSERT(m_buffer_pool);
	return *m_buffer_pool;
}

void global::make_io_runner(size_t concurrency)
{
	static remote::io_runner runner(concurrency);
	m_io_runner = &runner;
}

void global::make_buffer_pool(size_t size)
{
	static bindings::buffer_pool pool(size);
	m_buffer_pool = &pool;
}

}
