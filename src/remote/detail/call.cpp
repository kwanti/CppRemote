// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/call.hpp>


namespace remote
{
namespace detail
{

call::call()
: m_id(0)
, m_ready(false)
, m_object_id(0)
{}

call::call(object_vid const& oid)
: m_id(0)
, m_ready(false)
, m_object_id(oid)
{}

void call::set_exception(exception_ptr const& e)
{
	if(m_exception)
		return;

	m_exception = e;
	m_ready = true;
}

void call::invoke(pool_node_ptr const& node, service_ptr const& svc)
{
	BOOST_ASSERT(!m_ready);
	try
	{
		invoke(*node, *svc);
	}
	catch(...)
	{
		m_exception = current_exception();
	}
	m_ready = true;
}

}
}
