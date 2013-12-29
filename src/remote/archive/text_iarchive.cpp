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
#include <remote/archive/text_iarchive.hpp>
#include <boost/archive/detail/archive_serializer_map.hpp>

// explicitly instantiate for this type of text stream
#include <boost/archive/impl/archive_serializer_map.ipp>
#include <boost/archive/impl/basic_text_iarchive.ipp>
#include <remote/archive/text_iarchive_impl.ipp>


template class boost::archive::detail::archive_serializer_map<remote::archive::text_iarchive>;
template class boost::archive::basic_text_iarchive<remote::archive::text_iarchive>;
template class remote::archive::text_iarchive_impl<remote::archive::text_iarchive>;

