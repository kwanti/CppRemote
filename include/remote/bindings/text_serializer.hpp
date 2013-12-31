// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_TEXT_SERIALIZER_H__
#define __REMOTE_BINDINGS_TEXT_SERIALIZER_H__

#include <remote/archive/text_iarchive.hpp>
#include <remote/archive/text_oarchive.hpp>
#include <remote/bindings/basic_serializer.hpp>


namespace remote
{
namespace bindings
{

typedef basic_serializer<archive::text_iarchive, archive::text_oarchive> text_serializer;

}
}

#endif
