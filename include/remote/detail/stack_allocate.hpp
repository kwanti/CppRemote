// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_STACK_ALLOCATE_HPP__
#define __REMOTE_DETAIL_STACK_ALLOCATE_HPP__

#include <boost/aligned_storage.hpp>


namespace remote
{
namespace detail
{

template<typename T>
class stack_allocate
{
public:
	T* address()
	{
		return static_cast<T*>(m_storage.address());
	}

	T& reference()
	{
		return *address();
	}

private:
	typedef typename boost::aligned_storage<sizeof(T),
		boost::alignment_of<T>::value>::type storage_type;

	storage_type m_storage;
};

}
}

#endif
