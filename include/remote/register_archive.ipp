// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/archive_error.hpp>
#include <remote/remote_error.hpp>
#include <remote/system_error.hpp>
#include <remote/target_error.hpp>
#include <remote/filter_error.hpp>
#include <remote/serialization/system/asio_categories.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT(remote::filter_error)
BOOST_CLASS_EXPORT_IMPLEMENT(remote::archive_error)
BOOST_CLASS_EXPORT_IMPLEMENT(remote::remote_error)
BOOST_CLASS_EXPORT_IMPLEMENT(remote::system_error)
BOOST_CLASS_EXPORT_IMPLEMENT(remote::target_std_error)
BOOST_CLASS_EXPORT_IMPLEMENT(remote::target_unknown_error)

#define REMOTE_IMPLEMENT_CLASS remote::detail::comm_request
#include <remote/detail/comm_request.hpp>
