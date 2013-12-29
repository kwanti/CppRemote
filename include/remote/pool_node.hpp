// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_POOL_NODE_HPP__
#define __REMOTE_POOL_NODE_HPP__

#include <remote/fwd.hpp>
#include <boost/move/move.hpp>


namespace remote
{

class pool_node
{
    BOOST_MOVABLE_BUT_NOT_COPYABLE(pool_node)
public:

	pool_node();
	explicit pool_node(io_runner& runner);
	explicit pool_node(detail::pool_node_ptr const& node);

	pool_node(BOOST_RV_REF(pool_node) src)
	: m_node(boost::move(src.m_node))
	{}

	pool_node& operator = (BOOST_RV_REF(pool_node) src)
	{
		if(this != &src)
		{
			m_node = boost::move(src.m_node);
		}
		return *this;
	}


	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> bind(Target* target);

	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> bind(Target* target, std::string const& name);

	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> bind(boost::shared_ptr<Target> const& target);

	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> bind(boost::shared_ptr<Target> const& target, std::string const& name);

	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> bind(boost::weak_ptr<Target> const& target);

	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> bind(boost::weak_ptr<Target> const& target, std::string const& name);


	template<typename Proxy, typename Target>
	Proxy* bind_as_raw(Target* target);

	template<typename Proxy, typename Target>
	Proxy* bind_as_raw(Target* target, std::string const& name);

	template<typename Proxy, typename Target>
	Proxy* bind_as_raw(boost::shared_ptr<Target> const& target);

	template<typename Proxy, typename Target>
	Proxy* bind_as_raw(boost::shared_ptr<Target> const& target, std::string const& name);

	template<typename Proxy, typename Target>
	Proxy* bind_as_raw(boost::weak_ptr<Target> const& target);

	template<typename Proxy, typename Target>
	Proxy* bind_as_raw(boost::weak_ptr<Target> const& target, std::string const& name);


	template<typename Proxy, typename Target>
	void unbind(Target* target);

	template<typename Proxy, typename Target>
	void unbind(boost::shared_ptr<Target> const& target);

	template<typename Proxy>
	void unbind(Proxy* proxy);

	template<typename Proxy>
	void unbind(boost::shared_ptr<Proxy> const& proxy);

	void unbind(std::string const& name);


	void unbind_name(std::string const& name);
	void unbind_all();


	template<typename Target, typename Proxy>
	Target* target_cast(Proxy* p);

	template<typename Target, typename Proxy>
	boost::shared_ptr<Target> target_cast(boost::shared_ptr<Proxy> const& p);

	template<typename Target, typename Proxy>
	boost::weak_ptr<Target> target_cast(boost::weak_ptr<Proxy> const& p);


	template<typename Proxy, typename Target>
	Proxy* remote_cast(Target* t);

	template<typename Proxy, typename Target>
	boost::shared_ptr<Proxy> remote_cast(boost::shared_ptr<Target> const& t);

	template<typename Proxy, typename Target>
	boost::weak_ptr<Proxy> remote_cast(boost::weak_ptr<Target> const& t);

protected:
	detail::pool_node_ptr m_node;
};

}

#include <remote/pool_node.ipp>

#endif // __REMOTE_POOL_NODE_HPP__
