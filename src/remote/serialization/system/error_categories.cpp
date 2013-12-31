// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/serialization/system/error_categories.hpp>

#include <boost/bind/bind.hpp>
#include <boost/ref.hpp>
#include <algorithm>
#include <sstream>


namespace boost
{
namespace system
{

namespace
{

bool has_name(error_category const* cat, std::string const& name)
{
	BOOST_ASSERT(cat);
	return (name == cat->name());
}

class unknown_error_category : public error_category
{
public:
	unknown_error_category(){}

	char const* name() const { return "unknown"; }

	std::string message(int ev) const
	{
		std::stringstream ss;
		ss << "unknown error code with value = " << ev;
		return ss.str();
	}
};

}

BOOST_SYSTEM_DECL error_category const& unknown_category()
{
  static const unknown_error_category unknown_category_const;
  return unknown_category_const;
}

error_category const& error_categories::get(std::string const& name)
{
	std::set<error_category const*>& categories = get_categories();
	std::set<error_category const*>::iterator iter
		= std::find_if(categories.begin(), categories.end(),
						boost::bind(has_name, _1, boost::cref(name)));

	if(iter == categories.end())
		return unknown_category();

	return *(*iter);
}

error_categories::error_categories(error_category const& c1)
{
	get_categories().insert(&c1);
}

error_categories::error_categories(error_category const& c1, error_category const& c2)
{
	get_categories().insert(&c1);
	get_categories().insert(&c2);
}

error_categories::error_categories(error_category const& c1, error_category const& c2, error_category const& c3)
{
	get_categories().insert(&c1);
	get_categories().insert(&c2);
	get_categories().insert(&c3);
}

error_categories::error_categories(error_category const& c1, error_category const& c2, error_category const& c3, error_category const& c4)
{
	get_categories().insert(&c1);
	get_categories().insert(&c2);
	get_categories().insert(&c3);
	get_categories().insert(&c4);
}

error_categories system_categories(system_category(), generic_category());

}
}
