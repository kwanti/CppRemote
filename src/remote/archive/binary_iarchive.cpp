// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
#define BOOST_ARCHIVE_SOURCE
#include <remote/archive/binary_iarchive.hpp>
#include <boost/archive/detail/archive_serializer_map.hpp>

#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_binary_iprimitive.ipp>
#include <boost/archive/impl/basic_binary_iarchive.ipp>


// explicitly instantiate for this type of stream
template class boost::archive::detail::archive_serializer_map<remote::archive::binary_iarchive>;
template class boost::archive::basic_binary_iprimitive<
	remote::archive::binary_iarchive,
    std::istream::char_type,
    std::istream::traits_type
>;
template class boost::archive::basic_binary_iarchive<remote::archive::binary_iarchive> ;
template class boost::archive::binary_iarchive_impl<
	remote::archive::binary_iarchive,
    std::istream::char_type,
    std::istream::traits_type
>;
