// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_FUTURE_ERROR_HPP__
#define __REMOTE_FUTURE_ERROR_HPP__

#include <boost/detail/scoped_enum_emulation.hpp>
#include <boost/system/error_code.hpp>
#include <stdexcept>


namespace remote
{

BOOST_SCOPED_ENUM_DECLARE_BEGIN(future_errc)
{
	broken_promise,
	future_already_retrieved,
	promise_already_satisfied,
	no_state
}
BOOST_SCOPED_ENUM_DECLARE_END(future_errc)

boost::system::error_category const& future_category();

inline
boost::system::error_code make_error_code(future_errc e)
{
	using namespace boost::system;
	return error_code(boost::underlying_cast<int>(e), future_category());
}

inline
boost::system::error_condition make_error_condition(future_errc e)
{
	using namespace boost::system;
	return error_condition(boost::underlying_cast<int>(e), future_category());
}

class future_error
: public std::logic_error
{
public:
	future_error(boost::system::error_code ec)
	: std::logic_error(ec.message())
	, m_code(ec)
	{}

	boost::system::error_code const& code() const BOOST_NOEXCEPT
	{
		return m_code;
	}

private:
	boost::system::error_code m_code;
};

}

namespace boost
{
namespace system
{

template<>
struct is_error_code_enum<remote::future_errc>
{
	static const bool value = true;
};

#ifdef BOOST_NO_SCOPED_ENUMS
template<>
struct is_error_code_enum<remote::future_errc::enum_type>
{
	static const bool value = true;
};
#endif

}
}

#endif
