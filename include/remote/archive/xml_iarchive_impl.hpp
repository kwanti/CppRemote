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
#ifndef __REMOTE__ARCHIVE_XML_IARCHIVE_HPP__
#define __REMOTE__ARCHIVE_XML_IARCHIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <istream>

#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_xml_iarchive.hpp>
#include <boost/serialization/item_version_type.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header

namespace boost
{
namespace archive
{
template<class CharType>
class basic_xml_grammar;
}
}

namespace remote
{
namespace archive
{

typedef boost::archive::basic_xml_grammar<char> xml_grammar;

template<class Archive>
class xml_iarchive_impl
: public boost::archive::basic_text_iprimitive<std::istream>
, public boost::archive::basic_xml_iarchive<Archive>
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
    friend class boost::archive::detail::interface_iarchive<Archive>;
    friend class boost::archive::basic_xml_iarchive<Archive>;
    friend class boost::archive::load_access;
protected:
#endif
	// instances of micro xml parser to parse start preambles
	// scoped_ptr doesn't play nice with borland - so use a naked pointer
	// scoped_ptr<xml_grammar> gimpl;
	xml_grammar* gimpl;

	std::istream& get_is()
	{
		return is;
	}

	template<class T>
	void load(T& t)
	{
		boost::archive::basic_text_iprimitive<std::istream>::load(t);
	}

	void load(boost::archive::version_type& t)
	{
		unsigned int v;
		load(v);
		t = boost::archive::version_type(v);
	}

	void load(boost::serialization::item_version_type& t)
	{
		unsigned int v;
		load(v);
		t = boost::serialization::item_version_type(v);
	}

	BOOST_ARCHIVE_DECL(void) load(char* t);

#ifndef BOOST_NO_INTRINSIC_WCHAR_T
	BOOST_ARCHIVE_DECL(void) load(wchar_t* t);
#endif

	BOOST_ARCHIVE_DECL(void) load(std::string& s);

#ifndef BOOST_NO_STD_WSTRING
	BOOST_ARCHIVE_DECL(void) load(std::wstring& ws);
#endif

	template<class T>
	void load_override(T& t, BOOST_PFTO int)
	{
		boost::archive::basic_xml_iarchive<Archive>::load_override(t, 0);
	}

	BOOST_ARCHIVE_DECL(void) load_override(boost::archive::class_name_type& t, int);
	BOOST_ARCHIVE_DECL(void) init();

	BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) xml_iarchive_impl(std::istream& is, unsigned int flags);
	BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY()) ~xml_iarchive_impl();
};

} // namespace archive
} // namespace boost

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif
