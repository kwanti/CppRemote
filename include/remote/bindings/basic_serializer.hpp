// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_BASIC_SERIALIZER_H__
#define __REMOTE_BINDINGS_BASIC_SERIALIZER_H__

#include <remote/detail/call.hpp>
#include <remote/bindings/fwd.hpp>
#include <remote/bindings/buffer.hpp>
#include <remote/archive_error.hpp>
#include <remote/global.hpp>

#include <boost/archive/basic_archive.hpp>
#include <boost/noncopyable.hpp>
#include <boost/bind/bind.hpp>


namespace remote
{
namespace bindings
{

template<typename IArchive, typename OArchive>
class basic_serializer : public boost::noncopyable
{
	static const unsigned int archive_flag = boost::archive::no_tracking |
											 boost::archive::no_header |
											 boost::archive::no_codecvt |
											 boost::archive::no_xml_tag_checking;
public:
	buffer_ptr serialize(call_ptr const& _call)
	{
		BOOST_ASSERT(_call);
		try
		{
			buffer_ptr buf = global::buffer_pool().allocate();
			{
				std::ostream stream(buf.get());
				OArchive ar(stream, archive_flag);

				call* temp = _call.get();
				ar << boost::serialization::make_nvp("call", temp);
			}
			return buf;
		}
		catch(boost::archive::archive_exception& e)
		{
			throw archive_error(e);
		}
	}

	call_ptr deserialize(buffer_ptr const& buf)
	{
		BOOST_ASSERT(buf);
		try
		{
			std::istream stream(buf.get());
			IArchive ar(stream, archive_flag);

			call* temp = 0;
			ar >> boost::serialization::make_nvp("call", temp);
			return call_ptr(temp);
		}
		catch(boost::archive::archive_exception& e)
		{
			throw archive_error(e);
		}
	}
};

}
}

#endif
