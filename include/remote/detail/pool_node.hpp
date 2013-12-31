// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_POOL_NODE_HPP__
#define __REMOTE_DETAIL_POOL_NODE_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/service_pool.hpp>

#include <boost/smart_ptr/enable_shared_from_this.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/noncopyable.hpp>
#include <vector>


namespace remote
{
namespace detail
{

class direct_link;

class pool_node
: public boost::noncopyable
, public boost::enable_shared_from_this<pool_node>
{
public:
	pool_node(boost::uuids::uuid const& id);

	void initialize(boost::asio::io_service& ios);
	proxy_pool& get_proxy_pool();
	service_pool& get_service_pool();

	void register_pool(proxy_pool_ptr const& pool);
	void unregister_pool(proxy_pool_ptr const& pool);

	proxy_pool_ptr find_pool(boost::uuids::uuid const& id) const;
	proxy_pool_ptr find_pool(link_id_t id) const;

	/// TODO: add hints to specify which pool to select
	// when there are multiple connections between 2 nodes,
	// only the first found pool will be returned.
	// this method must be changed to add hints to specify which pool to select.

private:
	typedef boost::shared_ptr<direct_link> direct_link_ptr;
	typedef std::vector<proxy_pool_ptr> proxy_pools;

private:
	service_pool m_service_pool;
	direct_link_ptr m_internal_link;

	mutable boost::mutex m_mutex;
	proxy_pools m_proxy_pools;

private:
	static bool has_id(proxy_pool_ptr const& pool, boost::uuids::uuid const& id);
	static bool has_link_id(proxy_pool_ptr const& pool, link_id_t const& id);
};

}
}

#endif

