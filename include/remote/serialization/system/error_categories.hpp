// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_SERIALIZATION_SYSTEM_ERROR_CATEGORIES_HPP__
#define __REMOTE_SERIALIZATION_SYSTEM_ERROR_CATEGORIES_HPP__

#include <boost/system/error_code.hpp>
#include <set>


namespace boost
{
namespace system
{

class error_categories
{
	static std::set<error_category const*>& get_categories()
	{
		static std::set<error_category const*> categories;
		return categories;
	}

public:
	static error_category const& get(std::string const& name);

	error_categories(error_category const& c1);
	error_categories(error_category const& c1, error_category const& c2);
	error_categories(error_category const& c1, error_category const& c2, error_category const& c3);
	error_categories(error_category const& c1, error_category const& c2, error_category const& c3, error_category const& c4);
};

}
}

#define BOOST_ERROR_CATEGORIES_1(categories, c1)	\
namespace boost{	\
namespace system{	\
namespace{			\
error_categories categories(c1);	\
}}}

#define BOOST_ERROR_CATEGORIES_2(categories, c1, c2)	\
namespace boost{	\
namespace system{	\
namespace{			\
error_categories categories(c1, c2);	\
}}}

#define BOOST_ERROR_CATEGORIES_3(categories, c1, c2, c3)	\
namespace boost{	\
namespace system{	\
namespace{			\
error_categories categories(c1, c2, c3);	\
}}}

#define BOOST_ERROR_CATEGORIES_4(categories, c1, c2, c3, c4)	\
namespace boost{	\
namespace system{	\
namespace{			\
error_categories categories(c1, c2, c3, c4);	\
}}}

#endif // __REMOTE_SERIALIZATION_SYSTEM_ERROR_CATEGORIES_HPP__
