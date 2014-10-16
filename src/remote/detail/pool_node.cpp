// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/pool_node.hpp>
#include <remote/detail/proxy_pool.hpp>
#include <remote/detail/direct_link.hpp>

#include <boost/bind/bind.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/smart_ptr/make_shared.hpp>


namespace remote
{
namespace detail
{

pool_node::pool_node(boost::uuids::uuid const& id)
: m_service_pool(id)
{}

void pool_node::initialize(boost::asio::io_service& ios)
{
	BOOST_ASSERT(!m_internal_link);
	m_internal_link = boost::make_shared<direct_link>(shared_from_this(), boost::ref(ios));
	m_internal_link->initialize();
}

proxy_pool& pool_node::get_proxy_pool()
{
	return m_internal_link->get_proxy_pool();
}

service_pool& pool_node::get_service_pool()
{
	return m_service_pool;
}

void pool_node::register_pool(proxy_pool_ptr const& pool)
{
	BOOST_ASSERT(pool);

	boost::lock_guard<boost::mutex> lock(m_mutex);
	proxy_pools::iterator iter = std::find_if(m_proxy_pools.begin(), m_proxy_pools.end(),
									boost::bind(has_link_id, _1, pool->link_id()));
	if(iter == m_proxy_pools.end())
	{
		m_proxy_pools.push_back(pool);
		return;
	}

	BOOST_ASSERT(*iter == pool);
}

void pool_node::unregister_pool(proxy_pool_ptr const& pool)
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	proxy_pools::iterator iter = std::find_if(m_proxy_pools.begin(), m_proxy_pools.end(),
									boost::bind(has_link_id, _1, pool->link_id()));
	if(iter == m_proxy_pools.end())
		return;

	m_proxy_pools.erase(iter);
}

proxy_pool_ptr pool_node::find_pool(boost::uuids::uuid const& id) const
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	proxy_pools::const_iterator iter = std::find_if(m_proxy_pools.begin(), m_proxy_pools.end(),
										boost::bind(has_id, _1, boost::cref(id)));
	if(iter == m_proxy_pools.end())
		return proxy_pool_ptr();

	return *iter;
}

proxy_pool_ptr pool_node::find_pool(link_id_t id) const
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	proxy_pools::const_iterator iter = std::find_if(m_proxy_pools.begin(), m_proxy_pools.end(),
										boost::bind(has_link_id, _1, boost::cref(id)));
	if(iter == m_proxy_pools.end())
		return proxy_pool_ptr();

	return *iter;
}

bool pool_node::has_id(proxy_pool_ptr const& pool, boost::uuids::uuid const& id)
{
	return pool->id() == id;
}

bool pool_node::has_link_id(proxy_pool_ptr const& pool, link_id_t const& id)
{
	return pool->link_id() == id;
}

}
}
