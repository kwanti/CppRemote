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
#ifndef __REMOTE_ARCHIVE_TEXT_IARCHIVE_IMPL_HPP__
#define __REMOTE_ARCHIVE_TEXT_IARCHIVE_IMPL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <istream>

#include <boost/config.hpp>
#include <boost/archive/detail/auto_link_archive.hpp>
#include <boost/archive/basic_text_iprimitive.hpp>
#include <boost/archive/basic_text_iarchive.hpp>
#include <boost/serialization/item_version_type.hpp>

#include <boost/archive/detail/abi_prefix.hpp> // must be the last header


namespace remote
{
namespace archive
{

template<class Archive>
class text_iarchive_impl
: public boost::archive::basic_text_iprimitive<std::istream>
, public boost::archive::basic_text_iarchive<Archive>
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
	friend class boost::archive::detail::interface_iarchive<Archive>;
	friend class boost::archive::basic_text_iarchive<Archive>;
	friend class boost::archive::load_access;
protected:
#endif
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

	// note: the following should not needed - but one compiler (vc 7.1)
	// fails to compile one test (test_shared_ptr) without it !!!
	// make this protected so it can be called from a derived archive
	template<class T>
	void load_override(T& t, BOOST_PFTO int)
	{
		boost::archive::basic_text_iarchive<Archive>::load_override(t, 0);
	}

	BOOST_ARCHIVE_DECL(void) load_override(boost::archive::class_name_type& t, int);
	BOOST_ARCHIVE_DECL(void) init();

	BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY())
	text_iarchive_impl(std::istream& is, unsigned int flags);

	// don't import inline definitions! leave this as a reminder.
	//BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY())
	~text_iarchive_impl(){};
};

}
}

#include <boost/archive/detail/abi_suffix.hpp> // pops abi_suffix.hpp pragmas

#endif
