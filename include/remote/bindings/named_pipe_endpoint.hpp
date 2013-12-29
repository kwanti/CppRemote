// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_BINDINGS_NAMED_PIPE_ENDPOINT_HPP__
#define __REMOTE_BINDINGS_NAMED_PIPE_ENDPOINT_HPP__

#include <boost/asio/windows/stream_handle.hpp>
#ifdef BOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE

#include <string>


namespace remote
{
namespace bindings
{

class named_pipe_endpoint
{
public:
	static unsigned int const unlimited_connections = 255;
	static unsigned int const default_buffer_size = 512;

public:
	named_pipe_endpoint()
	: m_max_connections(unlimited_connections)
	, m_in_buffer_size(default_buffer_size)
	, m_out_buffer_size(default_buffer_size)
	{}

	explicit named_pipe_endpoint(std::string const& name)
	: m_max_connections(unlimited_connections)
	, m_in_buffer_size(default_buffer_size)
	, m_out_buffer_size(default_buffer_size)
	, m_path((!name.empty() && name[0] == '\\')? name: "\\\\.\\pipe\\" + name)
	{}

	bool operator == (named_pipe_endpoint const& rhs) const
	{
		return m_path == rhs.m_path;
	}

	char const* path() const { return m_path.c_str(); }
	unsigned int max_connections() const { return m_max_connections; }
	void set_max_connections(unsigned int value) { m_max_connections = value; }

	unsigned long in_buffer_size() const { return m_in_buffer_size; }
	void set_in_buffer_size(unsigned long value) { m_in_buffer_size = value; }

	unsigned long out_buffer_size() const { return m_out_buffer_size; }
	void set_out_buffer_size(unsigned long value) { m_out_buffer_size = value; }

private:
	unsigned int m_max_connections;
	unsigned long m_in_buffer_size;
	unsigned long m_out_buffer_size;
	std::string m_path;
};

}
}

#endif
#endif
