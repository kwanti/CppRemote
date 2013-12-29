// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#ifndef REMOTE_CLASS
#error "REMOTE_CLASS is not defined"
#endif

#ifndef REMOTE_REGISTER_CLASS
#error "REMOTE_REGISTER_CLASS is not defined"
#endif

#ifdef REMOTE_IMPLEMENT_CLASS
#define REMOTE_CLASS_EXPORT_IMPLEMENT(Call) BOOST_CLASS_EXPORT_IMPLEMENT(Call)
#else
#define REMOTE_CLASS_EXPORT_IMPLEMENT(Call)
#endif

#define REMOTE_REGISTER_CALL(Call)								\
BOOST_CLASS_TRACKING(Call, boost::serialization::track_never)	\
BOOST_CLASS_EXPORT_KEY(Call)									\
REMOTE_CLASS_EXPORT_IMPLEMENT(Call)

#include <remote/idl/detail/register_class.ipp>

#undef REMOTE_REGISTER_CALL
#undef REMOTE_CLASS_EXPORT_IMPLEMENT
#undef REMOTE_REGISTER_CLASS
#undef REMOTE_CLASS
#undef REMOTE_IMPLEMENT_CLASS
