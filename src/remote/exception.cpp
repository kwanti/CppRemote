// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/exception.hpp>
#include <remote/target_error.hpp>

#include <ostream>


namespace remote
{

exception_ptr current_exception()
{
	try
	{
		throw;
	}
	catch(exception& e)
	{
		return remote::make_exception_ptr(e);
	}
	catch(std::exception& e)
	{
		return boost::make_shared<target_std_error>(e);
	}
	catch(...)
	{
		return boost::make_shared<target_unknown_error>();
	}
}

std::ostream& operator << (std::ostream& os, exception const& e)
{
	os << e.what();
	return os;
}

}

