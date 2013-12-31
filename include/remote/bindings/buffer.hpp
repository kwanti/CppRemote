// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BUFFER_HPP__
#define __REMOTE_BINDINGS_BUFFER_HPP__

#include <boost/asio/streambuf.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>

namespace remote
{
namespace bindings
{

typedef boost::asio::basic_streambuf<> buffer;
typedef boost::shared_ptr<buffer> buffer_ptr;

}
}

#endif
