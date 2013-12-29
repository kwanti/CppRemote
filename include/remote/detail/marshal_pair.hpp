// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_MARSHAL_PAIR_HPP__
#define REMOTE_DETAIL_MARSHAL_PAIR_HPP__

#include <utility>


namespace remote
{
namespace detail
{

template<typename T1, typename T2>
struct is_remote_container<std::pair<T1, T2> >
: public boost::mpl::or_<is_any_remote_pointer<T1>, is_any_remote_pointer<T2>,
	is_remote_container<typename remove_any_pointer<T1>::type>,
	is_remote_container<typename remove_any_pointer<T2>::type> >
{};

template<typename T1, typename T2>
struct is_remote_container<std::pair<T1, T2> const>
: public boost::mpl::or_<is_any_remote_pointer<T1>, is_any_remote_pointer<T2>,
	is_remote_container<typename remove_any_pointer<T1>::type>,
	is_remote_container<typename remove_any_pointer<T2>::type> >
{};

template<typename T1, typename T2>
std::pair<T1, T2>& marshal_container(pool_node& node, std::pair<T1, T2>& c)
{
	marshal(node, c.first);
	marshal(node, c.second);
	return c;
}

}
}

#endif
