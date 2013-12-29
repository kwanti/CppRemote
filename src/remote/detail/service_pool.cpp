// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/service_pool.hpp>
#include <remote/detail/service_pool.ipp>

#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>


namespace remote
{
namespace detail
{

service_pool::service_pool(boost::uuids::uuid const& id)
: m_id(id)
{}

service_pool::~service_pool()
{}

object_id service_pool::resolve(std::string const& name) const
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
	named_services::const_iterator iter = m_named_services.find(name);
	if(iter == m_named_services.end())
		return 0;

	return iter->second;
}

object_id service_pool::resolve(object_vid const& name) const
{
	return boost::apply_visitor(resolve_visitor(*this), name);
}

service_ptr service_pool::find(object_id const& id) const
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
	services::const_iterator iter = m_services.find(id);
	if(iter == m_services.end())
		return service_ptr();

	return iter->second;
}

service_ptr service_pool::find(object_vid const& id) const
{
	return find(resolve(id));
}

void service_pool::unbind_name(std::string const& name)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
	m_named_services.erase(name);
}

void service_pool::unbind(std::string const& name)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);
	named_services::iterator iter = m_named_services.find(name);
	if(iter == m_named_services.end())
		return;

	unbind(iter->second);
}

void service_pool::unbind(object_vid const& vid)
{
	unbind_visitor visitor(*this);
	boost::apply_visitor(visitor, vid);
}

void service_pool::unbind(object_id const& id)
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	// remove all names bound to this id
	for(named_services::iterator iter = m_named_services.begin(),
		end = m_named_services.end(); iter != end;)
	{
		if(iter->second == id)
			m_named_services.erase(iter++);
		else
			++iter;
	}

	// remote the service
	m_services.erase(id);
	m_shared_targets.erase(id);
	m_unmanaged_targets.erase(id);
}

void service_pool::unbind_all()
{
	boost::lock_guard<boost::recursive_mutex> lock(m_mutex);

	m_named_services.clear();
	m_services.clear();
	m_shared_targets.clear();
	m_unmanaged_targets.clear();
}

}
}
