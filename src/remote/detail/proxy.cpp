// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/proxy.hpp>
#include <remote/detail/utility.hpp>
#include <remote/remote_error.hpp>

#include <boost/uuid/nil_generator.hpp>


namespace remote
{
namespace detail
{

proxy::proxy()
: m_pool_id(boost::uuids::nil_uuid())
, m_id(0)
, m_one_way_mode(false)
{}

proxy::proxy(pool_link_weak_ptr link, object_vid const& id)
: m_pool_link(link)
, m_pool_id(boost::uuids::nil_uuid())
, m_id(id)
, m_one_way_mode(false)
{}

proxy::~proxy()
{}

boost::uuids::uuid proxy::pool_id() const
{
	pool_link_ptr link = m_pool_link.lock();
	return link? link->pool_id(): m_pool_id;
}

void proxy::update()
{
	// update link info here
}

exception_ptr proxy::make_link_error()
{
	static remote_error e(remote_error::operation_aborted, "link disconnected error");
	return exception_ptr(&e, null_deleter());
}

}
}
