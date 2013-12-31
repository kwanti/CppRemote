// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_DIRECT_LINK_HPP__
#define __REMOTE_DETAIL_DIRECT_LINK_HPP__

#include <remote/detail/fwd.hpp>
#include <remote/detail/pool_link.hpp>

#include <boost/smart_ptr/enable_shared_from_this.hpp>


namespace remote
{
namespace detail
{

class direct_link
: public pool_link
, public boost::enable_shared_from_this<direct_link>
{
public:
	direct_link(pool_node_ptr const& pool, io_service& ios);
	~direct_link();

	void initialize();

	void send(call_ptr const& _call);
	object_id resolve(std::string const& name, bool& cache);
};

}
}

#endif

