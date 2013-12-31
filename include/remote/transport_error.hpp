// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_TRANSPORT_ERROR_HPP__
#define __REMOTE_TRANSPORT_ERROR_HPP__

#include <remote/exception.hpp>

namespace remote
{

class transport_error
: public virtual exception
{
public:
	virtual ~transport_error() throw() {}
};

}

#endif
