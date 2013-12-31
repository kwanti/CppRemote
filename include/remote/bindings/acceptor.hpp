// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_ACCEPTOR_HPP__
#define __REMOTE_BINDINGS_ACCEPTOR_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/fwd.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class acceptor : public boost::noncopyable
{
public:
	typedef boost::function<void(exception_ptr)> handler;

public:
	virtual void close() = 0;
	virtual bool is_listening() const = 0;
	virtual channel_ptr accept(handler) = 0;
};

}
}

#endif
