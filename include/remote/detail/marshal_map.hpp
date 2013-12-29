// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef REMOTE_DETAIL_MARSHAL_MAP_HPP__
#define REMOTE_DETAIL_MARSHAL_MAP_HPP__

#include <remote/detail/marshal_pair.hpp>
#include <map>


namespace remote
{
namespace detail
{

template<typename K, typename V, typename P, typename A>
struct is_remote_container<std::map<K, V, P, A> >
: public boost::mpl::or_<is_any_remote_pointer<K>, is_any_remote_pointer<V>,
	is_remote_container<typename remove_any_pointer<K>::type>,
	is_remote_container<typename remove_any_pointer<V>::type> >
{};

template<typename K, typename V, typename P, typename A>
struct is_remote_container<std::map<K, V, P, A> const>
: public boost::mpl::or_<is_any_remote_pointer<K>, is_any_remote_pointer<V>,
	is_remote_container<typename remove_any_pointer<K>::type>,
	is_remote_container<typename remove_any_pointer<V>::type> >
{};

template<typename K, typename V, typename P, typename A>
std::map<K, V, P, A>& marshal_container(pool_node& node, std::map<K, V, P, A>& c)
{
	std::map<K, V, P, A> temp;
	std::for_each(c.begin(), c.end(), marshal_into_(node, temp));
	c.swap(temp);

	return c;
}

}
}

#endif
