// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_SERVICE_POOL_HPP__
#define REMOTE_DETAIL_SERVICE_POOL_HPP__

#include <remote/detail/fwd.hpp>

#include <boost/variant/static_visitor.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/uuid/uuid.hpp>

#include <string>
#include <map>


namespace remote
{
namespace detail
{

class service_pool : public boost::noncopyable
{
public:
	explicit service_pool(boost::uuids::uuid const& id);
	~service_pool();

	boost::uuids::uuid id() const { return m_id; }

	object_id resolve(object_vid const& name) const;
	object_id resolve(std::string const& name) const;

	service_ptr find(object_vid const& id) const;
	service_ptr find(object_id const& id) const;


	template<typename Proxy, typename Target>
	object_id bind(Target* target, std::string const& name);

	template<typename Proxy, typename Target>
	object_id bind(Target* target);


	template<typename Proxy, typename Target>
	object_id bind(boost::shared_ptr<Target> const& target, std::string const& name);

	template<typename Proxy, typename Target>
	object_id bind(boost::shared_ptr<Target> const& target);


	template<typename Proxy, typename Target>
	object_id bind(boost::weak_ptr<Target> const& weak_target, std::string const& name);

	template<typename Proxy, typename Target>
	object_id bind(boost::weak_ptr<Target> const& weak_target);


	template<typename Proxy, typename Target>
	void unbind(Target* target);

	template<typename Proxy, typename Target>
	void unbind(boost::shared_ptr<Target> const& target);

	template<typename Proxy, typename Target>
	void unbind(boost::weak_ptr<Target> const& weak_target);


	template<typename Proxy>
	void unbind(Proxy* _proxy);

	template<typename Proxy>
	void unbind(boost::shared_ptr<Proxy> const& _proxy);

	template<typename Proxy>
	void unbind(boost::weak_ptr<Proxy> const& _proxy);


	void unbind_name(std::string const& name);
	void unbind(std::string const& name);
	void unbind(object_vid const& vid);
	void unbind(object_id const& id);
	void unbind_all();


	template<typename Target, typename Proxy>
	Target* target_cast(Proxy const* p);

	template<typename Target, typename Proxy>
	boost::shared_ptr<Target> target_cast(boost::shared_ptr<Proxy> const& p);

	template<typename Target, typename Proxy>
	boost::weak_ptr<Target> target_cast(boost::weak_ptr<Proxy> const& p);

private:
	typedef std::map<object_id, boost::shared_ptr<void> > targets;
	typedef std::map<object_id, service_ptr> services;
	typedef std::map<std::string, object_id> named_services;

	struct resolve_visitor
	: public boost::static_visitor<object_id>
	{
		resolve_visitor(service_pool const& pool)
		: m_pool(pool)
		{}

		object_id operator () (object_id const& id) const
		{
			return id;
		}

		object_id operator () (std::string const& name) const
		{
			return m_pool.resolve(name);
		}

	private:
		service_pool const& m_pool;

		resolve_visitor const& operator = (resolve_visitor const&);
	};

	struct unbind_visitor
	: public boost::static_visitor<void>
	{
		unbind_visitor(service_pool& pool)
		: m_pool(pool)
		{}

		void operator () (object_id const& id)
		{
			m_pool.unbind(id);
		}

		void operator () (std::string const& name)
		{
			m_pool.unbind(name);
		}

	private:
		service_pool& m_pool;

		unbind_visitor const& operator = (unbind_visitor const&);
	};

private:
	boost::uuids::uuid m_id;

	mutable boost::recursive_mutex m_mutex;
	named_services m_named_services;
	services m_services;
	targets m_unmanaged_targets;
	targets m_shared_targets;

private:
	template<typename Target>
	boost::shared_ptr<Target> get_unmanaged_target(Target* target, object_id const& id);

	template<typename Proxy, typename Target>
	object_id create_service(boost::shared_ptr<Target> const& target);

	template<typename Proxy, typename Target>
	object_id create_service(boost::weak_ptr<Target> const& weak_target);

private:
	template<typename Proxy, typename Target>
	static object_id unique_id(Target const* t);

	template<typename Proxy, typename Target>
	static object_id unique_id(boost::shared_ptr<Target> const& t);
};


}
}

#endif
