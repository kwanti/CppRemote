// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_PROXY_POOL_IPP__
#define __REMOTE_DETAIL_PROXY_POOL_IPP__

#include <remote/detail/pool_link.hpp>
#include <remote/detail/proxy.hpp>
#include <remote/remote_error.hpp>

#include <boost/variant/get.hpp>


namespace remote
{
namespace detail
{

template<typename Proxy>
Proxy* proxy_pool::get(raw_tag, std::string const& name, bool cache)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_unmanaged_proxies_mutex);
	boost::shared_ptr<Proxy> _proxy = get<Proxy>(shared_tag(), name, cache);
	if(!_proxy)
		return 0;

	BOOST_ASSERT(_proxy->id().which() == 0);	// must be object_id type

	m_unmanaged_proxies[boost::get<object_id>(_proxy->id())] = _proxy;
	return _proxy.get();
}

template<typename Proxy>
Proxy* proxy_pool::get(raw_tag, object_id const& id)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_unmanaged_proxies_mutex);
	boost::shared_ptr<Proxy> _proxy = get<Proxy>(shared_tag(), id);
	if(!_proxy)
		return 0;

	m_unmanaged_proxies[id] = _proxy;
	return _proxy.get();
}

template<typename Proxy>
Proxy* proxy_pool::get(raw_tag, object_vid const& id, bool cache)
{
	get_raw_visitor<Proxy> visitor(*this, cache);
	return boost::apply_visitor(visitor, id);
}

template<typename Proxy>
boost::shared_ptr<Proxy> proxy_pool::get(shared_tag, std::string const& name, bool cache) // nothrow
{
	pool_link_ptr link = m_weak_link.lock();
	if(!link)
	{
		return boost::shared_ptr<Proxy>();
	}

	object_id id = link->resolve(name, cache);
	if(id == 0)
	{
		BOOST_ASSERT(!cache);
		return boost::shared_ptr<Proxy>();
	}

	if(cache)
	{
		boost::shared_ptr<Proxy> _proxy = get<Proxy>(shared_tag(), id);
		if(_proxy)
			return _proxy;

		// retry to resolve name to non cached id
		cache = false;
		id = link->resolve(name, cache);
		if(id == 0)
		{
			return boost::shared_ptr<Proxy>();
		}
	}

	return get<Proxy>(shared_tag(), id);
}

template<typename Proxy>
boost::shared_ptr<Proxy> proxy_pool::get(shared_tag, object_id const& id)
{
	if(id == 0)
		return boost::shared_ptr<Proxy>();

	boost::lock_guard<boost::recursive_mutex> lock(m_proxies_mutex);

	proxies::iterator iter = m_proxies.find(id);
	if(iter == m_proxies.end())
	{
		// create new proxy
		boost::shared_ptr<Proxy> new_proxy = boost::make_shared<Proxy>(m_weak_link, id);
		m_proxies[id] = new_proxy;
		return new_proxy;
	}

	// id is found in the existing proxy, check to make sure it has the same interface
	boost::shared_ptr<Proxy> _proxy = boost::dynamic_pointer_cast<Proxy>(iter->second);
	if(!_proxy)
	{
		// requested interface is different from the existing proxy
		// this might happen because the name is resolved to an expired
		// version of cached id

		return boost::shared_ptr<Proxy>();
	}

	return _proxy;
}

template<typename Proxy>
boost::shared_ptr<Proxy> proxy_pool::get(shared_tag, object_vid const& id, bool cache)
{
	get_shared_visitor<Proxy> visitor(*this, cache);
	return boost::apply_visitor(visitor, id);
}

template<typename Proxy>
void proxy_pool::release(Proxy* _proxy)	// nothrow
{
	if(_proxy == 0)
		return;

	BOOST_ASSERT(_proxy->id().which() == 0);	// must be object_id type

	boost::lock_guard<boost::recursive_mutex> lock(m_unmanaged_proxies_mutex);
	proxies::iterator iter = m_unmanaged_proxies.find(boost::get<object_id>(_proxy->id()));
	if(iter == m_unmanaged_proxies.end())
		return;

	if(_proxy != iter->second.get())
		return;

	m_unmanaged_proxies.erase(iter);
}

template<typename Proxy>
void proxy_pool::release(boost::shared_ptr<Proxy> const& _proxy)	// nothrow
{
	if(!_proxy)
		return;

	BOOST_ASSERT(_proxy->id().which() == 0);	// must be object_id type
	object_id id = boost::get<object_id>(_proxy->id());

	boost::lock_guard<boost::recursive_mutex> lock(m_unmanaged_proxies_mutex);
	proxies::iterator iter = m_unmanaged_proxies.find(id);
	if(iter != m_unmanaged_proxies.end())
	{
		// won't release this shared pointer if user still using the raw pointer of this proxy
		return;
	}

	boost::lock_guard<boost::recursive_mutex> proxies_lock(m_proxies_mutex);
	proxies::iterator proxy_iter = m_proxies.find(id);
	if(proxy_iter == m_proxies.end())
		return;

	if(_proxy != proxy_iter->second)
		return;

	m_proxies.erase(proxy_iter);
}

}
}

#endif
