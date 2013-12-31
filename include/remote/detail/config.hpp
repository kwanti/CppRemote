// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <boost/config.hpp>

#ifndef BOOST_NOEXCEPT_OR_NOTHROW
#  ifdef BOOST_NO_NOEXCEPT
#    define BOOST_NOEXCEPT_OR_NOTHROW throw()
#  else
#    define BOOST_NOEXCEPT_OR_NOTHROW noexcept
#  endif
#endif
