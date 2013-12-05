// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v2 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDING_HPP__
#define __REMOTE_BINDING_HPP__

#include <remote/bindings/fwd.hpp>

namespace remote
{

class binding
{
public:
	binding()
	{}

	explicit binding(bindings::binding_ptr const& impl)
	: m_impl(impl)
	{}

private:
	friend class session;
	friend class server;

private:
	bindings::binding_ptr m_impl;
};

}

#endif
