// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under LGPL v2.1 license. You can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License version 2.1 as
// published by the Free Software Foundation. See <http://www.gnu.org/licenses/>.
//
// See www.cppremote.com for documentation.
//-----------------------------------------------------------------------------
#ifndef __REMOTE_ARCHIVE_POINTER_TRACKER_HPP__
#define __REMOTE_ARCHIVE_POINTER_TRACKER_HPP__

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/static_assert.hpp>
#include <boost/version.hpp>
#include <vector>

#if (BOOST_VERSION >= 105600)
#include <boost/serialization/singleton.hpp>	// workaround compile error in gcc mingw
#include <boost/serialization/type_info_implementation.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/shared_ptr_helper.hpp>
#endif

namespace remote
{
namespace archive
{

template<typename Archive>
class pointer_tracker
{
public:
	typedef std::vector<boost::shared_ptr<void const> > container_type;

public:
	void reset_container(container_type* container)
	{
		m_container = container;
	}

#if (BOOST_VERSION >= 105600)
	template<template<typename> class SPT, typename T>
	void reset(SPT<T>& sp, T* t)
	{
		reset_this(sp, t);
	}
private:
	template<template<typename> class SPT, typename T>
	void reset_this(SPT<T>& sp, T* t)
	{
		typedef boost::serialization::shared_ptr_helper<SPT> helper;
		this_archive()->template get_helper<helper>().reset(sp, t);
	}
#else
private:
	template<template<typename> class SPT, typename T>
	void reset_this(SPT<T>& sp, T* t)
	{
		this_archive()->reset(sp, t);
	}
#endif

protected:
	pointer_tracker()
	: m_container(0)
	{}

	template<typename T>
	void track_pointer(T& t)
	{
		track_pointer(t, typename boost::is_pointer<T>::type());
	}

private:
	container_type* m_container;

private:
	// return a pointer to the most derived class
	Archive* this_archive()
	{
		return static_cast<Archive*>(this);
	}

	template<typename TPtr>
	void track_pointer(TPtr& t, boost::mpl::true_)
	{
		typedef typename boost::remove_pointer<TPtr>::type T;
		BOOST_STATIC_ASSERT(boost::is_array<TPtr>::value == false);

		if((m_container == 0) || (t == 0))
			return;

		boost::shared_ptr<T> sp;
		reset_this(sp, t);

		BOOST_ASSERT(sp);
		BOOST_ASSERT(sp.get() == t);

		m_container->push_back(sp);
	}

	template<typename T>
	void track_pointer(T&, boost::mpl::false_)
	{}
};

}
}

#endif
