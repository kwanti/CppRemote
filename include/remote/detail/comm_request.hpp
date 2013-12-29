// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_COMM_REQUEST_HPP__
#define __REMOTE_DETAIL_COMM_REQUEST_HPP__

#include <remote/idl.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_serialize.hpp>


namespace remote
{
namespace detail
{

#define REMOTE_CLASS														\
REMOTE_CLASS_BEGIN(comm_request)											\
	REMOTE_METHOD_M1(boost::uuids::uuid, initialize, boost::uuids::uuid)	\
	REMOTE_METHOD_M1(object_id, resolve, std::string const&)				\
REMOTE_CLASS_END
#include <remote/idl/class.hpp>

}
}

#define REMOTE_REGISTER_CLASS remote::detail::comm_request
#include <remote/idl/register_class.hpp>

#endif
