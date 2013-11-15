// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v3 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_FWD_HPP__
#define __REMOTE_FWD_HPP__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/detail/scoped_enum_emulation.hpp>


namespace boost
{
namespace asio
{
class io_service;
}
}

namespace remote
{

class io_runner;

class session;
class server;
class binding;
class exception;

template<typename T>
class future;
template<typename T>
class shared_future;

BOOST_SCOPED_ENUM_DECLARE_BEGIN(future_status)
{
	ready, timeout, deferred
}
BOOST_SCOPED_ENUM_DECLARE_END(future_status)

typedef boost::asio::io_service io_service;

typedef boost::shared_ptr<exception> exception_ptr;

namespace detail
{

class server;
class proxy;
class pool_node;
class comm_link;

typedef boost::shared_ptr<server> server_ptr;
typedef boost::shared_ptr<proxy> proxy_ptr;
typedef boost::shared_ptr<comm_link> comm_link_ptr;
typedef boost::shared_ptr<pool_node> pool_node_ptr;

}

}

#endif

