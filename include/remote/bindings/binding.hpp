// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BINDING_HPP__
#define __REMOTE_BINDINGS_BINDING_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/fwd.hpp>

#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class binding
{
public:
	typedef boost::function<void(exception_ptr)> handler;

public:
	virtual channel_ptr connect(io_service&, handler) = 0;
	virtual acceptor_ptr listen(io_service&, handler) = 0;
};

}
}

#endif
