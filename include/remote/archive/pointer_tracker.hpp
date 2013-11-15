// Copyright 2013 Ng Kwan Ti <ngkwanti -at- gmail.com>
//
// This file is distributed under GPL v3 license. You can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3 as
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
#include <set>


namespace remote
{
namespace archive
{

template<typename Archive>
class pointer_tracker
{
public:
	typedef std::set<boost::shared_ptr<void const> > container_type;

public:
	void reset_container(container_type* container)
	{
		m_container = container;
	}

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
		this_archive()->reset(sp, t);

		BOOST_ASSERT(sp);
		BOOST_ASSERT(sp.get() == t);

		m_container->insert(sp);
	}

	template<typename T>
	void track_pointer(T&, boost::mpl::false_)
	{}
};

}
}

#endif
