// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/detail/direct_link.hpp>
#include <remote/detail/service_pool.hpp>
#include <remote/detail/pool_node.hpp>
#include <remote/global.hpp>

#include <boost/smart_ptr/make_shared.hpp>


namespace remote
{
namespace detail
{

direct_link::direct_link(pool_node_ptr const& node, io_service& ios)
: pool_link(node, ios)
{
	attach(node->get_service_pool().id());
}

direct_link::~direct_link()
{
	detach();
}

void direct_link::initialize()
{
	pool_link::initialize(shared_from_this());
}

void direct_link::send(call_ptr const& _call)
{
	m_io_service.post(boost::bind(&direct_link::dispatch, shared_from_this(), _call));
}

object_id direct_link::resolve(std::string const& name, bool& cache)
{
	cache = false;
	pool_node_ptr node = m_weak_node.lock();
	if(!node)
		return 0;

	return node->get_service_pool().resolve(name);
}

}
}
