// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_GLOBAL_H__
#define __REMOTE_GLOBAL_H__

#include <remote/bindings/buffer_pool.hpp>
#include <remote/io_runner.hpp>

#include <boost/thread/once.hpp>


namespace remote
{

class global
{
	global() {}
	global(global const&);
	void operator = (global const&);

public:
	static remote::io_runner& io_runner(size_t concurrency = 2);
	static bindings::buffer_pool& buffer_pool(size_t size = 3);

private:
	static void make_io_runner(size_t concurrency);
	static void make_buffer_pool(size_t size);

	static boost::once_flag m_io_runner_once;
	static boost::once_flag m_buffer_pool_once;

	static remote::io_runner* m_io_runner;
	static bindings::buffer_pool* m_buffer_pool;
};

}

#endif
