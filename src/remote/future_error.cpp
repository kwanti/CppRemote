// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/future_error.hpp>


namespace remote
{
namespace detail
{

class future_error_category
: public boost::system::error_category
{
public:
	virtual char const* name() const;
	virtual std::string message(int ev) const;
};

char const* future_error_category::name() const
{
	return "future";
}

std::string future_error_category::message(int ev) const
{
	switch(BOOST_SCOPED_ENUM_NATIVE(future_errc)(ev))
	{
	case future_errc::broken_promise:
		return std::string("The associated promise has been destructed prior \
							to the associated state becoming ready.");
	case future_errc::future_already_retrieved:
		return std::string("The future has already been retrieved from \
							the promise or packaged_task.");
	case future_errc::promise_already_satisfied:
		return std::string("The state of the promise has already been set.");
	case future_errc::no_state:
		return std::string("Operation not permitted on an object without \
							an associated state.");
	}
	return std::string("unspecified future_errc value\n");
}

}

boost::system::error_category const& future_category()
{
	static detail::future_error_category f;
	return f;
}

}
