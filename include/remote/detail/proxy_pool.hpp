// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_PROXY_POOL_HPP__
#define __REMOTE_DETAIL_PROXY_POOL_HPP__

#include <remote/detail/fwd.hpp>

#include <boost/variant/static_visitor.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/noncopyable.hpp>

#include <string>
#include <map>


namespace remote
{
namespace detail
{

typedef boost::mpl::true_ raw_tag;
typedef boost::mpl::false_ shared_tag;

class proxy_pool : public boost::noncopyable
{
public:
	proxy_pool(pool_link_ptr const& link);

	link_id_t link_id() const { return m_link_id; }

	boost::uuids::uuid id() const;
	void update();

	template<typename Proxy>
	Proxy* get(raw_tag, std::string const& name, bool cache = true);

	template<typename Proxy>
	Proxy* get(raw_tag, object_id const& id);

	template<typename Proxy>
	Proxy* get(raw_tag, object_vid const& id, bool cache = true);


	template<typename Proxy>
	boost::shared_ptr<Proxy> get(shared_tag, std::string const& name, bool cache = true);

	template<typename Proxy>
	boost::shared_ptr<Proxy> get(shared_tag, object_id const& id);

	template<typename Proxy>
	boost::shared_ptr<Proxy> get(shared_tag, object_vid const& id, bool cache = true);


	template<typename Proxy>
	void release(Proxy* _proxy);

	template<typename Proxy>
	void release(boost::shared_ptr<Proxy> const& _proxy);

	proxy_ptr find(std::string const& name) const;
	proxy_ptr find(object_id const& id) const;
	proxy_ptr find(object_vid const& id) const;

	void remove(std::string const& name);
	void remove(object_id const& id);
	void remove(object_vid const& id);

	void clear();
	void collect();

private:
	typedef std::map<object_id, proxy_ptr> proxies;

	template<typename Proxy>
	struct get_raw_visitor : boost::static_visitor<Proxy*>
	{
		get_raw_visitor(proxy_pool& pool, bool cache)
		: m_pool(pool)
		, m_cache(cache)
		{}

		Proxy* operator () (std::string const& id)
		{
			return m_pool.get<Proxy>(raw_tag(), id, m_cache);
		}

		Proxy* operator () (object_id const& id)
		{
			return m_pool.get<Proxy>(raw_tag(), id);
		}

		proxy_pool& m_pool;
		bool m_cache;

	private:
		get_raw_visitor const& operator = (get_raw_visitor const&);
	};

	template<typename Proxy>
	struct get_shared_visitor
	: public boost::static_visitor<boost::shared_ptr<Proxy> >
	{
		get_shared_visitor(proxy_pool& pool, bool cache)
		: m_pool(pool)
		, m_cache(cache)
		{}

		boost::shared_ptr<Proxy> operator () (std::string const& id)
		{
			return m_pool.get<Proxy>(shared_tag(), id, m_cache);
		}

		boost::shared_ptr<Proxy> operator () (object_id const& id)
		{
			return m_pool.get<Proxy>(shared_tag(), id);
		}

		proxy_pool& m_pool;
		bool m_cache;

	private:
		get_shared_visitor const& operator = (get_shared_visitor const&);
	};

	struct find_visitor
	: public boost::static_visitor<proxy_ptr>
	{
		find_visitor(proxy_pool const& pool)
		: m_pool(pool)
		{}

		proxy_ptr operator () (std::string const& id) const
		{
			return m_pool.find(id);
		}

		proxy_ptr operator () (object_id const& id) const
		{
			return m_pool.find(id);
		}

		proxy_pool const& m_pool;

	private:
		find_visitor const& operator = (find_visitor const&);
	};

private:
	link_id_t m_link_id;
	pool_link_weak_ptr m_weak_link;

	mutable boost::recursive_mutex m_proxies_mutex;
	proxies m_proxies;

	mutable boost::recursive_mutex m_unmanaged_proxies_mutex;
	proxies m_unmanaged_proxies;
};

}
}

#endif
