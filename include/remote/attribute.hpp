// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_ATTRIBUTE__
#define __REMOTE_ATTRIBUTE__
#include <remote/detail/proxy.hpp>


namespace remote
{

class attribute
{
public:
	static void set_one_way(detail::proxy* prx, bool one_way_mode)
	{
		BOOST_ASSERT(prx);
		prx->m_one_way_mode = one_way_mode;
	}

	static void set_one_way(detail::proxy_ptr prx, bool one_way_mode)
	{
		BOOST_ASSERT(prx);
		prx->m_one_way_mode = one_way_mode;
	}


	static bool get_one_way(detail::proxy* prx)
	{
		BOOST_ASSERT(prx);
		return prx->m_one_way_mode;
	}

	static bool get_one_way(detail::proxy_ptr prx)
	{
		BOOST_ASSERT(prx);
		return prx->m_one_way_mode;
	}

};

}


#endif
