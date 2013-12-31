// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_MAKE_TCPBINDING_HPP__
#define __REMOTE_MAKE_TCPBINDING_HPP__

#include <remote/bindings/text_serializer.hpp>
#include <remote/bindings/tcp_transport.hpp>
#include <remote/make_basic_binding.hpp>

namespace remote
{

template<typename A1>
binding make_tcp_binding(A1 a1)
{
	return make_basic_binding<bindings::text_serializer, bindings::tcp_transport>(a1);
}

template<typename A1, typename A2>
binding make_tcp_binding(A1 a1, A2 a2)
{
	return make_basic_binding<bindings::text_serializer, bindings::tcp_transport>(a1, a2);
}

}

#endif
