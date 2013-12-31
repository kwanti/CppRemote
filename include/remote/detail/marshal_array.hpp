// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_MARSHAL_ARRAY_HPP__
#define REMOTE_DETAIL_MARSHAL_ARRAY_HPP__

#include <boost/array.hpp>


namespace remote
{
namespace detail
{

template<typename T, std::size_t N>
struct is_remote_container<boost::array<T, N> >
: public boost::mpl::or_<is_any_remote_pointer<T>,
	is_remote_container<typename remove_any_pointer<T>::type> >
{};

template<typename T, std::size_t N>
struct is_remote_container<boost::array<T, N> const>
: public boost::mpl::or_<is_any_remote_pointer<T>,
	is_remote_container<typename remove_any_pointer<T>::type> >
{};

template<typename T, std::size_t N>
boost::array<T, N>& marshal_container(pool_node& node, boost::array<T, N>& c)
{
	std::for_each(c.begin(), c.end(), marshal_(node));
	return c;
}

}
}

#endif
