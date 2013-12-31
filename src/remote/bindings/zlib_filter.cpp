// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------

#include <remote/bindings/zlib_filter.hpp>
#include <remote/bindings/buffer.hpp>
#include <remote/filter_error.hpp>
#include <zlib.h>


namespace remote
{
namespace bindings
{

buffer_ptr zlib_filter::output(buffer_ptr const& buf)
{
	size_t buf_size = buf->size();
	buffer::mutable_buffers_type out = buf->prepare(compressBound(buf_size));
	Bytef* out_data = boost::asio::buffer_cast<Bytef*>(out);
	uLongf out_len = boost::asio::buffer_size(out);

	buffer::const_buffers_type in = buf->data();
	Bytef const* in_data = boost::asio::buffer_cast<Bytef const*>(in);
	uLong in_len = boost::asio::buffer_size(in);

	BOOST_ASSERT(in_len == buf_size);

	int ret = compress(out_data, &out_len, in_data, in_len);
	if(ret != Z_OK)
	{
		throw filter_error("fail in zlib compress");
	}

	buf->consume(in_len);
	buf->commit(out_len);

	return buf;
}

buffer_ptr zlib_filter::input(buffer_ptr const& buf)
{
	static const size_t chunk = 32;

	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.next_in = Z_NULL;
	strm.avail_in = 0;

	int ret = inflateInit(&strm);
	if(ret != Z_OK)
	{
		throw filter_error("fail inflateInit");
	}

	strm.avail_in = buf->size();
	do
	{
		size_t buf_size = strm.avail_in;

		strm.avail_out = chunk;
		strm.next_out = boost::asio::buffer_cast<Bytef*>(buf->prepare(chunk));
		strm.next_in = const_cast<Bytef*>(boost::asio::buffer_cast<Bytef const*>(buf->data()));
		int ret = inflate(&strm, Z_NO_FLUSH);
		if(ret < 0)
		break;

		buf->consume(buf_size - strm.avail_in);
		buf->commit(chunk - strm.avail_out);
	}
	while(strm.avail_in);

	if(ret < 0)
	{
		std::string msg = strm.msg;
		inflateEnd(&strm);
		throw filter_error(msg);
	}
	inflateEnd(&strm);

	return buf;
}

}
}
