// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_SERVICE_POOL_IPP__
#define REMOTE_DETAIL_SERVICE_POOL_IPP__

#include <remote/detail/type_traits.hpp>
#include <remote/detail/utility.hpp>
#include <remote/detail/type_id.hpp>
#include <remote/remote_error.hpp>

#include <boost/make_shared.hpp>


namespace remote
{
namespace detail
{

template<typename Proxy, typename Target>
object_id service_pool::unique_id(Target const* t)
{
	// assume 44-bit canonical address space
	static const object_id canonical_bit_mask = 0xfffffffffff;

	union
	{
		object_id m_object_id;
		struct part
		{
			object_id m_toid :44;	// target object id
			object_id m_ttid :10;	// target type id
			object_id m_ptid :10;	// proxy type id
		} m_part;
	} id;

	BOOST_STATIC_ASSERT(sizeof(id.m_object_id) >= sizeof(id.m_part));

	void const* void_ptr = t;
	if(void_ptr == 0)
		return 0;

	BOOST_ASSERT(reinterpret_cast<object_id>(void_ptr) <= canonical_bit_mask);

	id.m_object_id = 0;
	id.m_part.m_toid = reinterpret_cast<object_id>(void_ptr) & canonical_bit_mask;
	id.m_part.m_ttid = type_id<Target>::get();
	id.m_part.m_ptid = type_id<Proxy>::get();

	BOOST_ASSERT(id.m_part.m_ttid > 0);
	BOOST_ASSERT(id.m_part.m_ptid > 0);

	BOOST_ASSERT(id.m_part.m_ttid < 0x3ff);
	BOOST_ASSERT(id.m_part.m_ptid < 0x3ff);

	return id.m_object_id;
}

template<typename Proxy, typename Target>
object_id service_pool::unique_id(boost::shared_ptr<Target> const& t)
{
	return unique_id<Proxy>(t.get());
}

template<typename Target>
boost::shared_ptr<Target> service_pool::get_unmanaged_target(Target* target, object_id const& id)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	targets::iterator iter = m_unmanaged_targets.find(id);
	if(iter != m_unmanaged_targets.end())
	{
		// the id is unique by target type, so it should be safe to perform static cast
		return boost::static_pointer_cast<Target>(iter->second);
	}

	// the raw pointer is owned by caller.
	// we just wrapper it as a shared pointer that will not delete the pointer.
	boost::shared_ptr<Target> unmanaged_target(target, null_deleter());
	m_unmanaged_targets[id] = unmanaged_target;
	return unmanaged_target;
}

template<typename Proxy, typename Target>
object_id service_pool::bind(Target* target, std::string const& name)
{
	BOOST_ASSERT(target);
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	// bind the target
	object_id id = bind<Proxy>(target);
	BOOST_ASSERT(id);

	// update object name
	m_named_services[name] = id;

	return id;
}

template<typename Proxy, typename Target>
object_id service_pool::bind(Target* target)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	object_id id = unique_id<Proxy>(target);
	boost::shared_ptr<Target> unmanaged_target = get_unmanaged_target(target, id);
	return create_service<Proxy>(unmanaged_target);
}

template<typename Proxy, typename Target>
object_id service_pool::bind(boost::shared_ptr<Target> const& target, std::string const& name)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	// bind the target
	object_id id = create_service<Proxy>(target);
	if(id == 0)
		return 0;

	// hold the shared pointer
	m_shared_targets[id] = target;

	// update object name
	m_named_services[name] = id;

	return id;
}

template<typename Proxy, typename Target>
object_id service_pool::bind(boost::shared_ptr<Target> const& target)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	object_id id = create_service<Proxy>(target);
	if(id == 0)
		return 0;

	// hold the shared pointer
	m_shared_targets[id] = target;

	return id;
}

template<typename Proxy, typename Target>
object_id service_pool::bind(boost::weak_ptr<Target> const& weak_target, std::string const& name)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	// bind the target
	object_id id = create_service<Proxy>(weak_target);
	if(id == 0)
		return 0;

	// update object name
	m_named_services[name] = id;

	return id;
}

template<typename Proxy, typename Target>
object_id service_pool::bind(boost::weak_ptr<Target> const& weak_target)
{
	return create_service<Proxy>(weak_target);
}

template<typename Proxy, typename Target>
object_id service_pool::create_service(boost::shared_ptr<Target> const& target)
{
	return create_service<Proxy>(boost::weak_ptr<Target>(target));
}

template<typename Proxy, typename Target>
object_id service_pool::create_service(boost::weak_ptr<Target> const& weak_target)
{
	typedef typename to_service<Proxy, Target>::type service_type;

	boost::shared_ptr<Target> target = weak_target.lock();
	if(!target)
	{
		// throw here?
		return 0;
	}

	object_id id = unique_id<Proxy>(target);

	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	// check if same service has already bound to this pool
	services::iterator iter = m_services.find(id);
	if(iter == m_services.end())
	{
		// make a new service
		m_services[id] = boost::make_shared<service_type>(target, id);
	}
	else
	{
		// check service type
		boost::shared_ptr<service_type> found_service
			= boost::dynamic_pointer_cast<service_type>(iter->second);

		// object type to bind is different from the existing bound object
		// this should never happen because the id should be unique
		// by target pointer, interface type and target type.
		BOOST_ASSERT(found_service);
		BOOST_ASSERT(found_service->weak_target().lock() == target);
	}

	return id;
}

template<typename Proxy, typename Target>
void service_pool::unbind(Target* target)
{
	unbind(unique_id<Proxy>(target));
}

template<typename Proxy, typename Target>
void service_pool::unbind(boost::shared_ptr<Target> const& target)
{
	unbind(unique_id<Proxy>(target.get()));
}

template<typename Proxy, typename Target>
void service_pool::unbind(boost::weak_ptr<Target> const& weak_target)
{
	boost::shared_ptr<Target> target = weak_target.lock();
	if(!target)
		return;

	unbind(unique_id<Proxy>(target.get()));
}

template<typename Proxy>
void service_pool::unbind(Proxy* _proxy) // nothrow
{
	BOOST_ASSERT(_proxy);
	if(!_proxy)
		return;

	unbind(_proxy->id());
}

template<typename Proxy>
void service_pool::unbind(boost::shared_ptr<Proxy> const& _proxy)
{
	BOOST_ASSERT(_proxy);
	if(!_proxy)
		return;

	unbind(_proxy->id());
}

template<typename Proxy>
void service_pool::unbind(boost::weak_ptr<Proxy> const& weak_proxy)
{
	boost::shared_ptr<Proxy> _proxy = weak_proxy.lock();

	BOOST_ASSERT(_proxy);
	if(!_proxy)
		return;

	unbind(_proxy->id());
}

template<typename Target, typename Proxy>
Target* service_pool::target_cast(Proxy const* p)
{
	typedef typename to_service<Proxy, Target>::type service_type;

	if(!p)
		return 0;

	if(p->pool_id() != m_id)
		return 0;

	service_ptr svc = find(p->id());
	if(!svc)
		return 0;

	boost::shared_ptr<service_type> temp_svc
		= boost::dynamic_pointer_cast<service_type>(svc);
	if(!temp_svc)
		return 0;

	boost::shared_ptr<Target> t = temp_svc->weak_target().lock();
	if(!t)
		return 0;

	return t.get();
}

template<typename Target, typename Proxy>
boost::shared_ptr<Target> service_pool::target_cast(boost::shared_ptr<Proxy> const& sp)
{
	typedef typename to_service<Proxy, Target>::type service_type;

	if(!sp)
		return boost::shared_ptr<Target>();

	if(sp->pool_id() != m_id)
		return boost::shared_ptr<Target>();

	service_ptr svc = find(sp->id());
	if(!svc)
		return boost::shared_ptr<Target>();

	boost::shared_ptr<service_type> temp_svc
		= boost::dynamic_pointer_cast<service_type>(svc);
	if(!temp_svc)
		return boost::shared_ptr<Target>();

	return temp_svc->weak_target().lock();
}

template<typename Target, typename Proxy>
boost::weak_ptr<Target> service_pool::target_cast(boost::weak_ptr<Proxy> const& p)
{
	typedef typename to_service<Proxy, Target>::type service_type;

	boost::shared_ptr<Proxy> sp = p.lock();
	if(!sp)
		return boost::weak_ptr<Target>();

	if(sp->pool_id() != m_id)
		return boost::weak_ptr<Target>();

	service_ptr svc = find(sp->id());
	if(!svc)
		return boost::weak_ptr<Target>();

	boost::shared_ptr<service_type> temp_svc
		= boost::dynamic_pointer_cast<service_type>(svc);
	if(!temp_svc)
		return boost::weak_ptr<Target>();

	return temp_svc->weak_target();
}

}
}

#endif
