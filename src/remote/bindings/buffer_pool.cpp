// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v3 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#include <remote/bindings/buffer_pool.hpp>

#include <boost/smart_ptr/make_shared.hpp>


namespace remote
{
namespace bindings
{

buffer_pool::buffer_pool(size_t size)
: m_size(size)
{
	m_buffers.reserve(size);
}

buffer_ptr buffer_pool::allocate()
{
	{
		boost::lock_guard<boost::mutex> lock(m_mutex);
		buffers_type::iterator iter = std::find_if(m_buffers.begin(), m_buffers.end(), is_unique);
		if(iter != m_buffers.end())
		{
			buffer_ptr& buf(*iter);
			buf->consume(buf->size());
			return buf;
		}

		if(m_buffers.size() < m_size)
		{
			buffer_ptr buf = boost::make_shared<buffer>();
			m_buffers.push_back(buf);
			return buf;
		}
	}

	return boost::make_shared<buffer>();
}

void buffer_pool::collect()
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	buffers_type::iterator iter = std::remove_if(m_buffers.begin(), m_buffers.end(), is_unique);
	if(iter == m_buffers.end())
		return;

	m_buffers.erase(iter, m_buffers.end());
}

void buffer_pool::collect_one()
{
	boost::lock_guard<boost::mutex> lock(m_mutex);
	buffers_type::iterator iter = std::remove_if(m_buffers.begin(), m_buffers.end(), is_unique);
	if(iter == m_buffers.end())
		return;

	m_buffers.erase(iter);
}

bool buffer_pool::is_unique(buffer_ptr const& buf)
{
	return buf.unique();
}

}
}
