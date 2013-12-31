// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_CHANNEL_HPP__
#define __REMOTE_BINDINGS_CHANNEL_HPP__

#include <remote/fwd.hpp>
#include <remote/bindings/fwd.hpp>

#include <boost/noncopyable.hpp>
#include <boost/function.hpp>


namespace remote
{
namespace bindings
{

class channel : public boost::noncopyable
{
public:
	enum state_t { stopped, starting, started, stopping };
	typedef boost::function<void(exception_ptr)> handler;
	typedef boost::function<void(exception_ptr, call_ptr)> call_handler;

public:
	virtual state_t state() const = 0;
	virtual void disconnect(handler const&) = 0;
	virtual void send(call_ptr const&, handler const&) = 0;
	virtual void receive(call_handler const&) = 0;
};

}
}

#endif
