// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_FWD_HPP__
#define __REMOTE_DETAIL_FWD_HPP__

#include <remote/fwd.hpp>

#include <boost/smart_ptr/weak_ptr.hpp>
#include <boost/variant/variant_fwd.hpp>
#include <boost/cstdint.hpp>
#include <string>


namespace boost
{
namespace uuids
{
struct uuid;
}
}

namespace remote
{
namespace bindings
{
class binding;
class channel;
class acceptor;
}

namespace detail
{

typedef boost::uint32_t call_id;
typedef boost::uint64_t object_id;
typedef boost::variant<object_id, std::string> object_vid;
typedef std::size_t link_id_t;

struct one_way{};
struct const_tag{};
struct non_const_tag{};

class comm_request;

class proxy_pool;
class service_pool;
class pool_link;

class service;
class call;
class result;

template<typename R>
class basic_result;

typedef boost::shared_ptr<bindings::binding> binding_ptr;
typedef boost::shared_ptr<bindings::channel> channel_ptr;
typedef boost::shared_ptr<bindings::acceptor> acceptor_ptr;

typedef boost::shared_ptr<comm_request> comm_request_ptr;

typedef boost::shared_ptr<proxy_pool> proxy_pool_ptr;
typedef boost::shared_ptr<service_pool> service_pool_ptr;
typedef boost::shared_ptr<pool_link> pool_link_ptr;

typedef boost::shared_ptr<service> service_ptr;
typedef boost::shared_ptr<call> call_ptr;
typedef boost::shared_ptr<result> result_ptr;

typedef boost::weak_ptr<proxy> proxy_weak_ptr;
typedef boost::weak_ptr<result> result_weak_ptr;
typedef boost::weak_ptr<proxy_pool> proxy_pool_weak_ptr;
typedef boost::weak_ptr<service_pool> service_pool_weak_ptr;
typedef boost::weak_ptr<pool_link> pool_link_weak_ptr;
typedef boost::weak_ptr<pool_node> pool_node_weak_ptr;

}
}


#endif
