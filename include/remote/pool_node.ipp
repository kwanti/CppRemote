// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_POOL_NODE_IPP__
#define __REMOTE_POOL_NODE_IPP__

#include <remote/detail/pool_node.hpp>
#include <remote/detail/proxy_pool.hpp>
#include <remote/detail/service_pool.hpp>


namespace remote
{


template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::bind(Target* target)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target);
	return m_node->get_proxy_pool().get<Proxy>(detail::shared_tag(), id);
}

template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::bind(Target* target, std::string const& name)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target, name);
	return m_node->get_proxy_pool().get<Proxy>(detail::shared_tag(), id);
}

template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::bind(boost::shared_ptr<Target> const& target)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target);
	return m_node->get_proxy_pool().get<Proxy>(detail::shared_tag(), id);
}

template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::bind(boost::shared_ptr<Target> const& target, std::string const& name)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target, name);
	return m_node->get_proxy_pool().get<Proxy>(detail::shared_tag(), id);
}

template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::bind(boost::weak_ptr<Target> const& target)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target);
	return m_node->get_proxy_pool().get<Proxy>(detail::shared_tag(), id);
}

template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::bind(boost::weak_ptr<Target> const& target, std::string const& name)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target, name);
	return m_node->get_proxy_pool().get<Proxy>(detail::shared_tag(), id);
}


template<typename Proxy, typename Target>
Proxy* pool_node::bind_as_raw(Target* target)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target);
	return m_node->get_proxy_pool().get<Proxy>(detail::raw_tag(), id);
}

template<typename Proxy, typename Target>
Proxy* pool_node::bind_as_raw(Target* target, std::string const& name)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target, name);
	return m_node->get_proxy_pool().get<Proxy>(detail::raw_tag(), id);
}

template<typename Proxy, typename Target>
Proxy* pool_node::bind_as_raw(boost::shared_ptr<Target> const& target)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target);
	return m_node->get_proxy_pool().get<Proxy>(detail::raw_tag(), id);
}

template<typename Proxy, typename Target>
Proxy* pool_node::bind_as_raw(boost::shared_ptr<Target> const& target, std::string const& name)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target, name);
	return m_node->get_proxy_pool().get<Proxy>(detail::raw_tag(), id);
}

template<typename Proxy, typename Target>
Proxy* pool_node::bind_as_raw(boost::weak_ptr<Target> const& target)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target);
	return m_node->get_proxy_pool().get<Proxy>(detail::raw_tag(), id);
}

template<typename Proxy, typename Target>
Proxy* pool_node::bind_as_raw(boost::weak_ptr<Target> const& target, std::string const& name)
{
	detail::object_id id = m_node->get_service_pool().bind<Proxy>(target, name);
	return m_node->get_proxy_pool().get<Proxy>(detail::raw_tag(), id);
}


template<typename Proxy, typename Target>
void pool_node::unbind(Target* target)
{
	m_node->get_service_pool().unbind<Proxy>(target);
}

template<typename Proxy, typename Target>
void pool_node::unbind(boost::shared_ptr<Target> const& target)
{
	m_node->get_service_pool().unbind<Proxy>(target);
}

template<typename Proxy>
void pool_node::unbind(Proxy* proxy)
{
	m_node->get_service_pool().unbind(proxy);
}

template<typename Proxy>
void pool_node::unbind(boost::shared_ptr<Proxy> const& proxy)
{
	m_node->get_service_pool().unbind(proxy);
}


template<typename Target, typename Proxy>
Target* pool_node::target_cast(Proxy* p)
{
	return m_node->get_service_pool().target_cast<Target>(p);
}

template<typename Target, typename Proxy>
boost::shared_ptr<Target> pool_node::target_cast(boost::shared_ptr<Proxy> const& p)
{
	return m_node->get_service_pool().target_cast<Target>(p);
}

template<typename Target, typename Proxy>
boost::weak_ptr<Target> pool_node::target_cast(boost::weak_ptr<Proxy> const& p)
{
	return m_node->get_service_pool().target_cast<Target>(p);
}

template<typename Proxy, typename Target>
Proxy* pool_node::remote_cast(Target* t)
{
	return bind_as_raw<Proxy>(t);
}

template<typename Proxy, typename Target>
boost::shared_ptr<Proxy> pool_node::remote_cast(boost::shared_ptr<Target> const& t)
{
	return bind<Proxy>(t);
}

template<typename Proxy, typename Target>
boost::weak_ptr<Proxy> pool_node::remote_cast(boost::weak_ptr<Target> const& t)
{
	return bind<Proxy>(t);
}


}

#endif // __REMOTE_POOL_NODE_IPP__
