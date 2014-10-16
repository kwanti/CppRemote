// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
// (C) Copyright 2002 Robert Ramey - http://www.rrsd.com .
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_ARCHIVE_XML_IARCHIVE_HPP__
#define __REMOTE_ARCHIVE_XML_IARCHIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <remote/archive/xml_iarchive_impl.hpp>
#include <remote/archive/pointer_tracker.hpp>
#include <boost/archive/detail/register_archive.hpp>
#include <istream>


#if (BOOST_VERSION < 105600)

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4100)
#endif

// note special treatment of shared_ptr. This type needs a special
// structure associated with every archive.  We created a "mix-in"
// class to provide this functionality.  Since shared_ptr holds a
// special esteem in the boost library - we included it here by default.
#include <boost/archive/shared_ptr_helper.hpp>

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif

namespace remote
{
namespace archive
{

class xml_iarchive
: public xml_iarchive_impl<xml_iarchive>
#if (BOOST_VERSION < 105600)
, public boost::archive::detail::shared_ptr_helper
#endif
, public pointer_tracker<xml_iarchive>
{
public:
	xml_iarchive(std::istream& is, unsigned int flags = 0)
	: xml_iarchive_impl<xml_iarchive>(is, flags)
	{}

	template<typename T>
	void load_override(T& t, BOOST_PFTO int)
	{
		xml_iarchive_impl<xml_iarchive>::load_override(t, 0);
	}

	template<typename T>
	void load_override(
		#ifndef BOOST_NO_FUNCTION_TEMPLATE_ORDERING
		const
		#endif
		boost::serialization::nvp<T>& t, BOOST_PFTO int)
	{
		xml_iarchive_impl<xml_iarchive>::load_override(t, 0);
		track_pointer(t.value());
	}
};

}
}

BOOST_SERIALIZATION_REGISTER_ARCHIVE(remote::archive::xml_iarchive)

#endif
