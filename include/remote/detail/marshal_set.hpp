// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_MARSHAL_SET_HPP__
#define REMOTE_DETAIL_MARSHAL_SET_HPP__

#include <set>


namespace remote
{
namespace detail
{

template<typename T, typename C, typename A>
struct is_remote_container<std::set<T, C, A> >
: public boost::mpl::or_<is_any_remote_pointer<T>,
	is_remote_container<typename remove_any_pointer<T>::type> >
{};

template<typename T, typename C, typename A>
struct is_remote_container<std::set<T, C, A> const>
: public boost::mpl::or_<is_any_remote_pointer<T>,
	is_remote_container<typename remove_any_pointer<T>::type> >
{};

template<typename T, typename C, typename A>
std::set<T, C, A>& marshal_container(pool_node& node, std::set<T, C, A>& c)
{
	std::set<T, C, A> temp;
	std::for_each(c.begin(), c.end(), marshal_into_(node, temp));
	c.swap(temp);

	return c;
}

}
}

#endif
