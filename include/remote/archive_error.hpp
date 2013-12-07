// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v2 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_ARCHIVE_ERROR_HPP__
#define __REMOTE_ARCHIVE_ERROR_HPP__

#include <remote/exception.hpp>

#include <boost/archive/archive_exception.hpp>
#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

namespace remote
{

class archive_error
: public exception_base<archive_error>
, public boost::archive::archive_exception
{
public:
	archive_error()
	{
		BOOST_ASSERT(this == &this_ref());
	}

	archive_error(boost::archive::archive_exception const& e)
	: boost::archive::archive_exception(e)
	{
		BOOST_ASSERT(this == &this_ref());
	}

	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW
	{
		return boost::archive::archive_exception::what();
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive& ar, unsigned int const)
	{
		using namespace boost::serialization;
		base_object<remote::exception>(*this);
	}
};

inline exception_ptr make_archive_error(boost::archive::archive_exception const& e)
{
	return boost::make_shared<archive_error>(e);
}

}

BOOST_CLASS_TRACKING(remote::archive_error, boost::serialization::track_never)
BOOST_CLASS_EXPORT_KEY(remote::archive_error)

#endif
