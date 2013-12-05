// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v2 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_FWD_HPP__
#define __REMOTE_BINDINGS_FWD_HPP__

#include <boost/smart_ptr/shared_ptr.hpp>


namespace remote
{
namespace detail
{
class call;
}

namespace bindings
{

class binding;
class acceptor;
class channel;
typedef detail::call call;

typedef boost::shared_ptr<binding> binding_ptr;
typedef boost::shared_ptr<acceptor> acceptor_ptr;
typedef boost::shared_ptr<channel> channel_ptr;
typedef boost::shared_ptr<call> call_ptr;

}
}

namespace boost
{
namespace asio
{
class io_service;
}
}

#endif
