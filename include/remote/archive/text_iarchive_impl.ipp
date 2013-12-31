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

//////////////////////////////////////////////////////////////////////
// implementation of basic_text_iprimitive overrides for the combination
// of template parameters used to implement a text_iprimitive

#include <cstddef> // size_t, NULL
#include <boost/config.hpp>
#if defined(BOOST_NO_STDC_NAMESPACE)
namespace std
{
    using ::size_t;
} // namespace std
#endif

#include <boost/detail/workaround.hpp> // RogueWave

#include <remote/archive/text_iarchive_impl.hpp>


namespace remote
{
namespace archive
{

template<class Archive>
BOOST_ARCHIVE_DECL(void) text_iarchive_impl<Archive>::load(char *s)
{
	std::size_t size;
	*this->This() >> size;

	// skip separating space
	is.get();

	// Works on all tested platforms
	is.read(s, size);
	s[size] = '\0';
}

template<class Archive>
BOOST_ARCHIVE_DECL(void) text_iarchive_impl<Archive>::load(std::string &s)
{
	std::size_t size;
	*this->This() >> size;

	// skip separating space
	is.get();

	// borland de-allocator fixup
#if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
	if(NULL != s.data())
#endif
		s.resize(size);

	if(0 < size)
		is.read(&(*s.begin()), size);
}

#ifndef BOOST_NO_CWCHAR
#ifndef BOOST_NO_INTRINSIC_WCHAR_T

template<class Archive>
BOOST_ARCHIVE_DECL(void) text_iarchive_impl<Archive>::load(wchar_t *ws)
{
	std::size_t size;
	* this->This() >> size;

	// skip separating space
	is.get();
	is.read((char *)ws, size * sizeof(wchar_t)/sizeof(char));
	ws[size] = L'\0';
}
#endif // BOOST_NO_INTRINSIC_WCHAR_T

#ifndef BOOST_NO_STD_WSTRING
template<class Archive>
BOOST_ARCHIVE_DECL(void) text_iarchive_impl<Archive>::load(std::wstring &ws)
{
	std::size_t size;
	* this->This() >> size;

	// borland de-allocator fixup
	#if BOOST_WORKAROUND(_RWSTD_VER, BOOST_TESTED_AT(20101))
	if(NULL != ws.data())
	#endif
		ws.resize(size);

	// skip separating space
	is.get();
	is.read((char *)ws.data(), size * sizeof(wchar_t)/sizeof(char));
}

#endif // BOOST_NO_STD_WSTRING
#endif // BOOST_NO_CWCHAR

template<class Archive>
BOOST_ARCHIVE_DECL(void)
text_iarchive_impl<Archive>::load_override(boost::archive::class_name_type& t, int)
{
    boost::archive::basic_text_iarchive<Archive>::load_override(t, 0);
}

template<class Archive>
BOOST_ARCHIVE_DECL(void) text_iarchive_impl<Archive>::init()
{
	boost::archive::basic_text_iarchive<Archive>::init();
}

template<class Archive>
BOOST_ARCHIVE_DECL(BOOST_PP_EMPTY())
text_iarchive_impl<Archive>::text_iarchive_impl(std::istream & is, unsigned int flags)
: boost::archive::basic_text_iprimitive<std::istream>(is, (flags & boost::archive::no_codecvt) != 0)
, boost::archive::basic_text_iarchive<Archive>(flags)
{
	if((flags & boost::archive::no_header) == 0)
#if BOOST_WORKAROUND(__MWERKS__, BOOST_TESTED_AT(0x3205))
		this->init();
#else
		this->boost::archive::basic_text_iarchive<Archive>::init();
#endif
}

} // namespace archive
} // namespace boost
