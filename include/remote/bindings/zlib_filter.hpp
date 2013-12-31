// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_ZLIB_FILTER_HPP__
#define __REMOTE_BINDINGS_ZLIB_FILTER_HPP__

#include <remote/bindings/fwd.hpp>


namespace remote
{
namespace bindings
{

class zlib_filter
{
public:
	buffer_ptr output(buffer_ptr const& buf);
	buffer_ptr input(buffer_ptr const& buf);
};

}
}

#endif
