// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v2 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_MAKE_IPCBINDING_HPP__
#define __REMOTE_MAKE_IPCBINDING_HPP__

#include <remote/bindings/binary_serializer.hpp>
#include <remote/bindings/ipc_transport.hpp>
#include <remote/make_basic_binding.hpp>

namespace remote
{

template<typename A1>
binding make_ipc_binding(A1 a1)
{
	return make_basic_binding<bindings::binary_serializer, bindings::ipc_transport>(a1);
}

}

#endif
