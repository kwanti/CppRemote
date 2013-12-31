// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_SERIALIZATION_SYSTEM_ERROR_CODE_HPP__
#define __REMOTE_SERIALIZATION_SYSTEM_ERROR_CODE_HPP__

#include <remote/serialization/system/error_categories.hpp>

#include <boost/serialization/split_free.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>


BOOST_SERIALIZATION_SPLIT_FREE(boost::system::error_code)

namespace boost
{
namespace serialization
{

template<typename Archive>
void save(Archive& ar, boost::system::error_code const& ec, unsigned int const)
{
	int value = ec.value();
	std::string cat = ec.category().name();

	ar << make_nvp("value", value);
	ar << make_nvp("category", cat);
}

template<typename Archive>
void load(Archive& ar, boost::system::error_code& ec, unsigned int const)
{
	int value;
	std::string cat;

	ar >> make_nvp("value", value);
	ar >> make_nvp("category", cat);

	ec.assign(value, boost::system::error_categories::get(cat));
}

}
}

#endif
