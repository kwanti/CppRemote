// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_SYSTEM_ERROR_HPP__
#define __REMOTE_SYSTEM_ERROR_HPP__

#include <remote/exception.hpp>
#include <remote/serialization/system/error_code.hpp>

#include <boost/system/system_error.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>


namespace remote
{

class system_error
: public exception_base<system_error>
, public boost::system::system_error
{
public:
	system_error(boost::system::error_code const& code)
	: boost::system::system_error(code)
	{
		BOOST_ASSERT(this == &this_ref());
	}

	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW
	{
		return boost::system::system_error::what();
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive&, const unsigned int)
	{
		boost::serialization::base_object<remote::exception>(*this);
	}
};

inline exception_ptr make_system_error(boost::system::error_code const& e)
{
	return boost::make_shared<system_error>(e);
}

}

namespace boost
{
namespace serialization
{

template<typename Archive>
void save_construct_data(Archive& ar, remote::system_error const* e, unsigned int const)
{
	boost::system::error_code code(e->code());
	ar << make_nvp("code", code);
}

template<typename Archive>
void load_construct_data(Archive& ar, remote::system_error* e, unsigned int const)
{
	boost::system::error_code code;
	ar >> make_nvp("code", code);
	::new(e)remote::system_error(code);
}

}
}

BOOST_CLASS_TRACKING(remote::system_error, boost::serialization::track_never)
BOOST_CLASS_EXPORT_KEY(remote::system_error)
#endif
