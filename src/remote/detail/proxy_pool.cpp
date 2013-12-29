// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/detail/proxy_pool.hpp>
#include <remote/detail/proxy_pool.ipp>

#include <boost/uuid/nil_generator.hpp>


namespace remote
{
namespace detail
{

proxy_pool::proxy_pool(pool_link_ptr const& link)
: m_link_id(link->id())
, m_weak_link(link)
{}

boost::uuids::uuid proxy_pool::id() const
{
	pool_link_ptr link = m_weak_link.lock();
	return (link)? link->pool_id(): boost::uuids::nil_uuid();
}

void proxy_pool::update()
{
	boost::lock_guard<boost::recursive_mutex> lock(m_proxies_mutex);

	// update all proxies
	for(proxies::iterator iter = m_proxies.begin(); iter != m_proxies.end(); ++iter)
	{
//		iter->second->update();
	}
}

proxy_ptr proxy_pool::find(std::string const& name) const
{
	pool_link_ptr link = m_weak_link.lock();
	if(!link)
	{
		std::cout << "no link to find proxy" << std::endl;
		return proxy_ptr();
	}

	bool cache = true;
	object_id id = link->resolve(name, cache);
	if(id == 0)
	{
		std::cout << "link unable to resolve name to proxy" << std::endl;
		return proxy_ptr();
	}

	return find(id);
}

proxy_ptr proxy_pool::find(object_id const& id) const
{
	boost::lock_guard<boost::recursive_mutex> lock(m_proxies_mutex);

	proxies::const_iterator iter = m_proxies.find(id);
	if(iter == m_proxies.end())
	{
		std::cout << "proxy not exist" << std::endl;
		return proxy_ptr();
	}

	return iter->second;
}

proxy_ptr proxy_pool::find(object_vid const& id) const
{
	return boost::apply_visitor(find_visitor(*this), id);
}

void proxy_pool::remove(object_id const& id)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_proxies_mutex);
	m_proxies.erase(id);
}

void proxy_pool::clear()
{
	boost::lock_guard<boost::recursive_mutex> lock(m_proxies_mutex);
	m_proxies.clear();
}

void proxy_pool::collect()
{
	// collect all managed proxy that is not reference by other
	// where it's reference count = 1
}

}
}
