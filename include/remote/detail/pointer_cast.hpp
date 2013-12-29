// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_DETAIL_POINTER_CAST_HPP__
#define __REMOTE_DETAIL_POINTER_CAST_HPP__

#include <boost/pointer_cast.hpp>
#include <boost/version.hpp>

#include <boost/smart_ptr/shared_ptr.hpp>

namespace boost
{

#if (BOOST_VERSION < 105300)
template<class T, class U>
shared_ptr<T> reinterpret_pointer_cast(shared_ptr<U> const& r) BOOST_NOEXCEPT
{
    (void) reinterpret_cast< T* >( static_cast< U* >( 0 ) );

	typedef typename shared_ptr<T>::element_type E;

    E * p = reinterpret_cast< E* >( r.get() );
	return shared_ptr<T>( r, p );
}
#endif


// pointer cast overload for weak_ptr
template<class T, class U>
weak_ptr<T> dynamic_pointer_cast(weak_ptr<U> const& r) BOOST_NOEXCEPT
{
	return dynamic_pointer_cast<T>(r.lock());
}

template<class T, class U>
weak_ptr<T> const_pointer_cast(weak_ptr<U> const& r) BOOST_NOEXCEPT
{
	return const_pointer_cast<T>(r.lock());
}

}



#endif // __REMOTE_DETAIL_POINTER_CAST_HPP__
