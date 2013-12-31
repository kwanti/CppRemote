// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_FILTER_ERROR_HPP__
#define __REMOTE_FILTER_ERROR_HPP__

#include <remote/exception.hpp>

#include <boost/serialization/export.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>
#include <stdexcept>


namespace remote
{

class filter_error
: public exception_base<filter_error>
, public std::runtime_error
{
public:
	filter_error(std::string const& msg)
	: std::runtime_error(msg.c_str())
	{
		BOOST_ASSERT(this == &this_ref());
	}

	virtual char const* what() const BOOST_NOEXCEPT_OR_NOTHROW
	{
		return std::runtime_error::what();
	}

private:
	friend class boost::serialization::access;

	template<typename Archive>
	void serialize(Archive&, unsigned int const)
	{
		boost::serialization::base_object<remote::exception>(*this);
	}
};

inline exception_ptr make_filter_error(std::string const& msg)
{
	return boost::make_shared<filter_error>(msg);
}

}

namespace boost
{
namespace serialization
{

template<typename Archive>
void save_construct_data(Archive& ar, remote::filter_error const* e, unsigned int const)
{
	std::string msg(e->what());

	ar << make_nvp("msg", msg);
}

template<typename Archive>
void load_construct_data(Archive& ar, remote::filter_error* e, unsigned int const)
{
	std::string msg;

	ar >> make_nvp("msg", msg);

	::new(e)remote::filter_error(msg);
}

}
}

BOOST_CLASS_TRACKING(remote::filter_error, boost::serialization::track_never)
BOOST_CLASS_EXPORT_KEY(remote::filter_error)

#endif
