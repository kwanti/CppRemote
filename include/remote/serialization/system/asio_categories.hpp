// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_SERIALIZATION_SYSTEM_ASIO_CATEGORIES_HPP__
#define __REMOTE_SERIALIZATION_SYSTEM_ASIO_CATEGORIES_HPP__

#include <remote/serialization/system/error_categories.hpp>
#include <boost/asio/error.hpp>

BOOST_ERROR_CATEGORIES_3(asio_categories,
				boost::asio::error::netdb_category,
				boost::asio::error::addrinfo_category,
				boost::asio::error::misc_category)

#endif // __REMOTE_SERIALIZATION_SYSTEM_ASIO_CATEGORIES_HPP__
