// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_MAKE_BASICBINDING_HPP__
#define __REMOTE_MAKE_BASICBINDING_HPP__

#include <remote/bindings/basic_binding.hpp>
#include <remote/binding.hpp>

#include <boost/smart_ptr/make_shared.hpp>


namespace remote
{

template<typename Serializer, typename Transport, typename A1>
binding make_basic_binding(A1 a1)
{
	typedef bindings::basic_binding<Serializer, Transport> binding_type;
	typedef typename Transport::endpoint_type endpoint_type;

	return binding(boost::make_shared<binding_type>(endpoint_type(a1)));
}

template<typename Serializer, typename Transport, typename Filter, typename A1>
binding make_basic_binding(A1 a1)
{
	typedef bindings::basic_binding<Serializer, Transport, Filter> binding_type;
	typedef typename Transport::endpoint_type endpoint_type;

	return binding(boost::make_shared<binding_type>(endpoint_type(a1)));
}

template<typename Serializer, typename Transport, typename A1, typename A2>
binding make_basic_binding(A1 a1, A2 a2)
{
	typedef bindings::basic_binding<Serializer, Transport> binding_type;
	typedef typename Transport::endpoint_type endpoint_type;

	return binding(boost::make_shared<binding_type>(endpoint_type(a1, a2)));
}

template<typename Serializer, typename Transport, typename Filter, typename A1, typename A2>
binding make_basic_binding(A1 a1, A2 a2)
{
	typedef bindings::basic_binding<Serializer, Transport, Filter> binding_type;
	typedef typename Transport::endpoint_type endpoint_type;

	return binding(boost::make_shared<binding_type>(endpoint_type(a1, a2)));
}

}

#endif
