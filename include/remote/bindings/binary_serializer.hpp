// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BINARY_SERIALIZER_H__
#define __REMOTE_BINDINGS_BINARY_SERIALIZER_H__

#include <remote/archive/binary_iarchive.hpp>
#include <remote/archive/binary_oarchive.hpp>
#include <remote/bindings/basic_serializer.hpp>


namespace remote
{
namespace bindings
{

typedef basic_serializer<archive::binary_iarchive, archive::binary_oarchive> binary_serializer;

}
}

#endif
